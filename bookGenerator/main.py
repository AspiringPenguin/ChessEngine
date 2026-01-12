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
            counter += 1
            processNode(game, None)


        pgnFile.close()
        print("Closed: game-data/" + file)

except KeyboardInterrupt:
    print(f"Exited with {counter} games added. Generating book...")

with open("book.txt", mode="w") as f:
    for zHash in positions.keys():
        occurences = len(positions[zHash])

        if zHash == 0x463b96181691fc9c: #Start position
            minCount = max(int(occurences * 0.01), 1000)
        elif zHash in [0x823c9b50fd114196, 0x830eb9b20758d1de, 0x9d5f7aee7e779da1, 0xca18093c559e579b]: #After 1. e4, d4, Nf3, c4 - black can play many different options here - don't suppress
            minCount = max(int(occurences * 0.02), 1000)
        else:
            minCount = max(int(occurences * 0.05), 1000)

        first = True
        
        for move in sorted(set(positions[zHash]), key=lambda x: positions[zHash].count(x), reverse=True): #For each move in that position
            if positions[zHash].count(move) < minCount: #Ignore rare moves
                break
            
            if first:
                f.write(str(zHash))
                first = False
            
            f.write(";" + move.uci() + "," + str(positions[zHash].count(move)))

        if not first: #ie a move has been written
            f.write("\n")