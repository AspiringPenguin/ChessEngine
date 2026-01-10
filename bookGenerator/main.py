from types import NoneType
import chess
import chess.pgn
import chess.polyglot
import os

positions = {} #Hash to list of moves

def processNode(game : chess.GameNode | NoneType, zHash : int | NoneType):
    if game == None:
        return

    if zHash:
        if zHash in positions:
            positions[zHash].append(game.move)
        else:
            positions[zHash] = [game.move]

    if game.ply() > 10:
        return

    zHash = chess.polyglot.zobrist_hash(game.board())

    processNode(game.next(), zHash)

counter = 0

try:
    for file in os.listdir("game-data/"):
        print("Opening file: game-data/" + file)
        pgnFile = open("game-data/" + file)

        while game := chess.pgn.read_game(pgnFile):
            if (int(game.headers["WhiteElo"]) > 1800) and (int(game.headers["BlackElo"]) > 1800):
                counter += 1
                processNode(game, None)


        pgnFile.close()
        print("Closed: game-data/" + file)

except KeyboardInterrupt:
    print(f"Exited with {counter} games added.")

while True:
    inp = int(input("> "), 16)
    for move in sorted(set(positions[inp]), key=lambda x: positions[inp].count(x), reverse=True):
        print(move.uci(), positions[inp].count(move))