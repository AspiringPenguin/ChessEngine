from chess import pgn, Board
import os
import re

import time

annotatedFens = list[str]()

def processGame(game : pgn.Game, gameID : int):
    res = game.headers.get("Result", "?")
    if res == "1-0":
        gameScore = 1
    elif res == "0-1":
        gameScore = -1
    else:
        gameScore = 0

    processNode(game, game.board(), gameScore, gameID)

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

    start = time.perf_counter()
    
    tuningGames = open(file, mode="r")

    totalGamesInFile = len(re.findall("Event", tuningGames.read()))

    print(f"Started {file}, containing {totalGamesInFile} games.")

    tuningGames.seek(0)

    while game := pgn.read_game(tuningGames):
        counter += 1
        gameID += 1

        processGame(game, gameID)

        if (counter % 500) == 0:
            print(f"{counter} games processed.")
    print(f"{counter} games processed.")


    tuningGames.close()

    end = time.perf_counter()

    totalGames += totalGamesInFile

    print(f"Finished file {file}, with {counter} games processed, in {end - start}s.")
    print(f"Total games: {totalGames}, total positions: {len(annotatedFens)}.")
    print()

with open("positions.txt", mode="w") as f:
    for annotatedFen in annotatedFens:
        f.write(f"{annotatedFen}\n")
