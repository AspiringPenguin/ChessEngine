from types import NoneType
import chess
import chess.pgn
import chess.polyglot
import os

positions = {} #Hash to dictionary of moves and frequencies

def processNode(game : chess.GameNode | NoneType, zHash : int | NoneType):
    if game == None:
        return

    if zHash:
        if zHash in positions:
            if game.move in positions[zHash]:
                positions[zHash][game.move] += 1
            else:
                positions[zHash][game.move] = 1
        else:
            positions[zHash] = {game.move: 1}

    if game.ply() > 16:
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
        occurences = 0
        for key in positions[zHash].keys():
            occurences += positions[zHash][key]

        if zHash == 0x463b96181691fc9c: #Start position
            minCount = int(occurences * 0.1)
        else:
            minCount = max(int(occurences * 0.3), 10)

        first = True
        
        for move in positions[zHash].keys(): #For each move in that position
            if positions[zHash][move] < minCount: #Ignore rare moves
                continue
            
            if first:
                f.write(str(zHash))
                first = False
            
            f.write(";" + move.uci() + "," + str(positions[zHash][move]))

        if not first: #ie a move has been written
            f.write("\n")