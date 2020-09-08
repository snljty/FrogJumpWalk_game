#! /usr/bin/env python3
# -*- Coding: UTF-8 -*-

r"""
This is a game.
"""

print("This program is a game of frogs jumping or walking.")
print("")
print("The frogs facing right can only go right.")
print("The frogs facing  left can only go  left.")
print("There us a blank at the center at te begining.")
print("If a frog is on the side of the blank position, ")
print("Then it can walk to the blank, ")
print("as long as it is on the correct direction.")
print("Or if a frog is separated by another frog from the blank, ")
print("it can jump over to the blank if in the correct direction.")
print("")
print("Your aim is to swap all frogs facing left and all facing right.")
print("Click Buttons on the GUI window to move the frogs.")
print("Or you can use the keyboard to control the movements.")
print("Press 'f' to walk a frog right, 'd' to jump a frog right, ")
print("'j' to walf a frog right and 'k' to jump a frog left.")
print("Click the 'Hint' button or press 'h' to show or hide the hint.")
print("Click the 'Exit' button or press 'q' to exit.")
print("Click the 'Reset' button or press 'r' to reset the game.")
print("")

print("Input the number of frogs on each side (default: 3): ")
try:
    nFrogs = int(input())
except ValueError:
    nFrogs = 3
finally:
    print("")
assert nFrogs > 0, 'At least one frog should be set!'

import os, sys
import tkinter as tk
from PIL import Image, ImageTk

window = tk.Tk()
window.title('Frog Walk or Jump')
window.geometry('%dx250' % (100 * (2 * nFrogs + 1)))
window.resizable(0, 0) 

frameCanvas = tk.Frame(
    master = window, 
    height = 100, 
    width = 100 * (2 * nFrogs + 1), 
)
frameCanvas.place(
    x = 0, 
    y = 0, 
    anchor = tk.NW
)

frameButton = tk.Frame(
    master = window, 
    height = 150, 
    width = 100 * (2 * nFrogs + 1), 
)
frameButton.place(
    x = 0, 
    y = 100, 
    anchor = tk.NW
)

canvas = tk.Canvas(
    master = frameCanvas, 
    height = 100, 
    width = 100 * (2 * nFrogs + 1)
)

canvas.place(
    x = 0, 
    y = 0, 
    anchor = tk.NW
)

# need to make sure 'frog.png' is under current folder.
frogImagePath = os.path.join(os.path.dirname(sys.argv[0]), 'frog.png')
if not os.path.isfile(frogImagePath):
    raise ValueError("Error! Image '%s' not found." % frogImagePath)
imageRight = Image.open(frogImagePath)
imageLeft  = imageRight.transpose(Image.FLIP_LEFT_RIGHT)
imageTkRight = ImageTk.PhotoImage(image = imageRight)
imageTkLeft  = ImageTk.PhotoImage(image = imageLeft)

def GenerateHint(n: int, reverse: bool = False) -> list:
    l = list()
    if reverse:
        operators = ['-->', '<--', '->', '<-']
    else:
        operators = ['<--', '-->', '<-', '->']
    for i in range(1, n + 1):
        for j in range(1, i + 1):
            if j == i:
                l.append(operators[2 + i % 2])
            else:
                l.append(operators[i % 2])
    for j in range(n):
        l.append(operators[(n + 1) % 2])
    for i in range(n, 0, -1):
        for j in range(i, 0, -1):
            if j == i:
                l.append(operators[2 + i % 2])
            else:
                l.append(operators[i % 2])
    return l

def InitGame():
    global nFrogs, board, nStep, solution, hintInfo
    global canvas, imageTkRight, imageTkLeft, blankPos, boardInit
    board = list()
    for i in range(nFrogs):
        board.append(
            canvas.create_image(
                100 * i, 
                0, 
                anchor = tk.NW, 
                image = imageTkRight
            )
        )
    board.append(0)
    for i in range(nFrogs):
        board.append(
            canvas.create_image(
                100 * (i + nFrogs + 1), 
                0, 
                anchor = tk.NW, 
                image = imageTkLeft
            )
        )
    blankPos = nFrogs
    nStep = 0 # the nth step of the correct movement
    solution = str()
    hintInfo = '->/<-'
    return

InitGame()

def ResetGame():
    global nFrogs, board, canvas, blankPos, nStep, solution, hintInfo, labelHintText
    for i in range(1, len(board)):
        if i <= nFrogs: # reset frogs towards right.
            # reset image position
            canvas.coords(i, 100 * (i - 1), 0)
            # reset board
            board[i - 1] = i
        else: # reset frogs towards left.
            # reset image position
            canvas.coords(i, 100 * i, 0)
            # reset board
            board[i] = i
    # reset blankPos
    board[nFrogs] = 0
    blankPos = nFrogs
    # reset current step
    nStep = 0
    solution = str()
    hintInfo = '->/<-'
    if labelHintText.get():
        labelHintText.set(hintInfo)
    return

def IsFinished() -> bool:
    global board
    for i in range(nFrogs):
        if board[i] <= nFrogs:
            return False
    if board[nFrogs]:
        return False
    return True

def PrintFinishGame():
    global buttonHint, labelHintText, buttonReset, frameButton
    print("Congratulations! You finished the game successfully!")
    print("Close the GUI window to exit.")
    frameButton.unbind(sequence = '<KeyPress-h>')
    buttonHint.destroy()
    frameButton.unbind(sequence = '<KeyPress-r>')
    buttonReset.destroy()
    labelHintText.set('Congratulations!')
    return

def WalkRight():
    global board, blankPos, nFrogs, canvas, solution, nStep, hintInfo, labelHintText
    if (blankPos == 0) or (board[blankPos - 1] > nFrogs):
        print('\a', end = '')
        return
    canvas.move(board[blankPos - 1], 100, 0)
    board[blankPos], board[blankPos - 1] = board[blankPos - 1], board[blankPos]
    blankPos -= 1
    if nStep >= 0:
        nStep += 1
    if nStep == 1:
        solution = GenerateHint(nFrogs, reverse = False)
        hintInfo = solution[nStep]
        if labelHintText.get():
            labelHintText.set(hintInfo)
    elif 1 < nStep < len(solution):
        if hintInfo == '->':
            hintInfo = solution[nStep]
            if labelHintText.get():
                labelHintText.set(hintInfo)
        else:
            nStep = -1
            solution = str()
            hintInfo = 'Reset'
            if labelHintText.get():
                labelHintText.set(hintInfo)
    elif nStep == len(solution):
        hintInfo = str()
    if IsFinished():
        PrintFinishGame()
    return

def WalkLeft():
    global board, blankPos, nFrogs, canvas, solution, nStep, hintInfo, labelHintText
    if (blankPos == 2 * nFrogs) or (board[blankPos + 1] <= nFrogs):
        print('\a', end = '')
        return
    canvas.move(board[blankPos + 1], -100, 0)
    board[blankPos], board[blankPos + 1] = board[blankPos + 1], board[blankPos]
    blankPos += 1
    if nStep >= 0:
        nStep += 1
    if nStep == 1:
        solution = GenerateHint(nFrogs, reverse = True)
        hintInfo = solution[nStep]
        if labelHintText.get():
            labelHintText.set(hintInfo)
    elif 1 < nStep < len(solution):
        if hintInfo == '<-':
            hintInfo = solution[nStep]
            if labelHintText.get():
                labelHintText.set(hintInfo)
        else:
            nStep = -1
            solution = str()
            hintInfo = 'Reset'
            if labelHintText.get():
                labelHintText.set(hintInfo)
    elif nStep == len(solution):
        hintInfo = str()
    if IsFinished():
        PrintFinishGame()
    return

def JumpRight():
    global board, blankPos, nFrogs, canvas, solution, nStep, hintInfo, labelHintText
    if (blankPos <= 1) or (board[blankPos - 2] > nFrogs):
        print('\a', end = '')
        return
    canvas.move(board[blankPos - 2], 200, 0)
    board[blankPos], board[blankPos - 2] = board[blankPos - 2], board[blankPos]
    blankPos -= 2
    if nStep >= 0:
        nStep += 1
    if nStep == 1:
        nStep = -1
        solution = str()
        hintInfo = 'Reset'
        if labelHintText.get():
            labelHintText.set(hintInfo)
    elif 1 < nStep < len(solution):
        if hintInfo == '-->':
            hintInfo = solution[nStep]
            if labelHintText.get():
                labelHintText.set(hintInfo)
        else:
            nStep = -1
            solution = str()
            hintInfo = 'Reset'
            if labelHintText.get():
                labelHintText.set(hintInfo)
    elif nStep == len(solution):
        hintInfo = str()
    if IsFinished():
        PrintFinishGame()
    return

def JumpLeft():
    global board, blankPos, nFrogs, canvas, solution, nStep, hintInfo, labelHintText
    if (blankPos >= 2 * nFrogs - 1) or (board[blankPos + 2] <= nFrogs):
        print('\a', end = '')
        return
    canvas.move(board[blankPos + 2], -200, 0)
    board[blankPos], board[blankPos + 2] = board[blankPos + 2], board[blankPos]
    blankPos += 2
    if nStep >= 0:
        nStep += 1
    if nStep == 1:
        nStep = -1
        solution = str()
        hintInfo = 'Reset'
        if labelHintText.get():
            labelHintText.set(hintInfo)
    elif 1 < nStep < len(solution):
        if hintInfo == '<--':
            hintInfo = solution[nStep]
            if labelHintText.get():
                labelHintText.set(hintInfo)
        else:
            nStep = -1
            solution = str()
            hintInfo = 'Reset'
            if labelHintText.get():
                labelHintText.set(hintInfo)
    elif nStep == len(solution):
        hintInfo = str()
    if IsFinished():
        PrintFinishGame()
    return

buttonWalkRight = tk.Button(
    master = frameButton, 
    text = '(f)Walk->', 
    width = 10, 
    height = 2, 
    command = WalkRight
)
buttonWalkLeft = tk.Button(
    master = frameButton, 
    text = '<-Walk(j)', 
    width = 10, 
    height = 2, 
    command = WalkLeft
)
buttonJumpRight = tk.Button(
    master = frameButton, 
    text = '(d)Jump-->', 
    width = 10, 
    height = 2, 
    command = JumpRight
)
buttonJumpLeft = tk.Button(
    master = frameButton, 
    text = '<--Jump(k)', 
    width = 10, 
    height = 2, 
    command = JumpLeft
)

buttonWalkRight.place(
    x = 50, 
    y = 25, 
    anchor = tk.CENTER
)
buttonWalkLeft.place(
    x = nFrogs * 200 + 50, 
    y = 25, 
    anchor = tk.CENTER
)
buttonJumpRight.place(
    x = 50, 
    y = 75, 
    anchor = tk.CENTER
)
buttonJumpLeft.place(
    x = nFrogs * 200 + 50, 
    y = 75, 
    anchor = tk.CENTER
)

frameText = tk.Frame(
    master = frameButton, 
    height = 50, 
    width = 100 * (2 * nFrogs - 1)
)
frameText.place(
    x = nFrogs * 100 + 50, 
    y = 75, 
    anchor = tk.CENTER
)

labelHintText = tk.StringVar(master = frameText)
labelHint = tk.Label(
    master = frameText, 
    height = 2, 
    width = 10 * (2 * nFrogs - 1), 
    textvariable = labelHintText
)
labelHint.place(
    x = nFrogs * 100 - 50, 
    y = 25, 
    anchor = tk.CENTER
)

def HintShowHide():
    global labelHintText, hintInfo
    if labelHintText.get():
        labelHintText.set(str())
    else:
        labelHintText.set(hintInfo)
    return

buttonHint = tk.Button(
    master = frameButton, 
    text = 'Hint(h)', 
    width = 10, 
    height = 2, 
    command = HintShowHide
)
buttonHint.place(
    x = nFrogs * 100 + 50, 
    y = 25, 
    anchor = tk.CENTER
)

buttonReset = tk.Button(
    master = frameButton, 
    text = 'Reset(r)', 
    width = 10, 
    height = 2, 
    command = ResetGame
)
buttonReset.place(
    x = 75, 
    y = 125, 
    anchor = tk.CENTER
)

buttonExit = tk.Button(
    master = frameButton, 
    text = 'Exit(q)', 
    width = 10, 
    height = 2, 
    command = window.destroy
)
buttonExit.place(
    x = nFrogs * 200 + 25, 
    y = 125, 
    anchor = tk.CENTER
)

frameButton.bind(
    sequence = '<KeyPress-f>', 
    func = lambda event: WalkRight()
)
frameButton.bind(
    sequence = '<KeyPress-j>', 
    func = lambda event: WalkLeft()
)
frameButton.bind(
    sequence = '<KeyPress-d>', 
    func = lambda event: JumpRight()
)
frameButton.bind(
    sequence = '<KeyPress-k>', 
    func = lambda event: JumpLeft()
)
frameButton.bind(
    sequence = '<KeyPress-r>', 
    func = lambda event: ResetGame()
)
frameButton.bind(
    sequence = '<KeyPress-q>', 
    func = lambda event: window.destroy()
)
frameButton.bind(
    sequence = '<KeyPress-h>', 
    func = lambda event: HintShowHide()
)
frameButton.focus_set()

window.mainloop()

