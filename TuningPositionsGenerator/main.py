from chess import pgn, Board
import os

annotatedFens = list[str]()

def processGame(game : pgn.Game):
    res = game.headers.get("Result", "?")
    if res == "1-0":
        gameScore = 1
    elif res == "0-1":
        gameScore = -1
    else:
        gameScore = 0

    processNode(game, game.board(), gameScore)

def processNode(game : pgn.GameNode, board: Board, result : int, ply: int = 0):
    isMateScore = "M" in game.comment
    if ply >= 16 and not isMateScore:
        #Process this node
        fen = board.fen(en_passant="fen")

        annotatedFen = f"{fen};{result}"

        annotatedFens.append(annotatedFen)
    
    nextPos = game.next()

    if nextPos and not isMateScore:
        nextMove = nextPos.move
        board.push(nextMove)
        processNode(nextPos, board, result, ply+1)

counter = 0

for file in os.listdir():
    if not file.endswith(".pgn"):
        continue

    print(file)
    
    tuningGames = open(file, mode="r")

    while game := pgn.read_game(tuningGames):
        counter += 1
        processGame(game)

    tuningGames.close()

    print(f"Finished file {file}, with {counter} games processed.")

with open("positions.txt", mode="w") as f:
    for annotatedFen in annotatedFens:
        f.write(f"{annotatedFen}\n")