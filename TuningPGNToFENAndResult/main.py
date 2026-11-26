from chess import pgn, Board
import os
import re

import time

annotatedFens = list[str]()

otherEngineNames = ["stash"]

def processGame(game : pgn.Game, gameID : int) -> bool:
    white, black = game.headers.get("White", "?"), game.headers.get("Black", "?")

    for otherEngine in otherEngineNames:
        if otherEngine in white.lower() or otherEngine in black.lower():
            return False

    res = game.headers.get("Result", "?")
    if res == "1-0":
        gameScore = 1
    elif res == "0-1":
        gameScore = -1
    else:
        gameScore = 0

    processNode(game, game.board(), gameScore, gameID)
    return True

def processNode(game : pgn.GameNode, board: Board, result : int, gameID : int, ply: int = 0):
    isMateScore = "M" in game.comment
    #The cheap checks
    if ply >= 16 and not isMateScore:
        #Now check the position is quiet
        inCheck = board.is_check()
        if (not inCheck):
            #Process this node
            fen = board.fen(en_passant="fen")

            annotatedFen = f"{fen};{gameID};{result}"

            annotatedFens.append(annotatedFen)
    
    nextPos = game.next()

    if nextPos and not isMateScore and ply < 200: #Ply > 200 ie move > 100 tends to be shuffling leading to 50-move rule draw
        nextMove = nextPos.move
        board.push(nextMove)
        processNode(nextPos, board, result, gameID, ply+1)

totalGames = 0
gameID = -1

for file in os.listdir():
    if not file.endswith(".pgn"):
        continue

    counter = 0
    skipped = 0

    start = time.perf_counter()
    
    tuningGames = open(file, mode="r")

    totalGames = len(re.findall("Event", tuningGames.read()))

    print(f"Started {file}, containing {totalGames} games.")

    tuningGames.seek(0)

    lastGameProcessed = True

    while game := pgn.read_game(tuningGames):
        if lastGameProcessed:
            counter += 1
            gameID += 1
        else:
            skipped += 1

        lastGameProcessed = processGame(game, gameID)

        if (counter % 500) == 0 and lastGameProcessed:
            print(f"{counter} games processed, {skipped} games skipped.")

    if not lastGameProcessed:
        skipped += 1


    tuningGames.close()

    end = time.perf_counter()

    totalGames += counter

    print(f"Finished file {file}, with {counter} games processed and {skipped} games skipped, in {end - start}s.")
    print(f"Total games processed: {totalGames}, total positions: {len(annotatedFens)}.")
    print()

with open("positions.txt", mode="w") as f:
    for annotatedFen in annotatedFens:
        f.write(f"{annotatedFen}\n")
