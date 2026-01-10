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
    print(f"Exited with {counter} games added. Continuing...")

with open("book.txt", mode="w") as f:
    for zHash in positions.keys():
        if len(positions[zHash]) < 100: #Skip rare positions
            continue

        first = True
        
        for move in sorted(set(positions[zHash]), key=lambda x: positions[zHash].count(x), reverse=True): #For each move in that position
            if positions[zHash].count(move) < 100: #Ignore rare moves
                break
            
            if first:
                f.write(str(zHash))
                first = False
            
            f.write(";" + move.uci() + "," + str(positions[zHash].count(move)))

        if not first: #ie a move has been written
            f.write("\n")



#For my own curiosity, a polyglot-hash based explorer exists here
while True:
    inp = int(input("> "), 16)
    for move in sorted(set(positions[inp]), key=lambda x: positions[inp].count(x), reverse=True):
        print(move.uci(), positions[inp].count(move))