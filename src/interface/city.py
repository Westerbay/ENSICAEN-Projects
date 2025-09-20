from cell import Cell

import pygame


"""

ENSICAEN
6 Boulevard Maréchal Juin
F-14050 Caen Cedex
 
This file is owned by ENSICAEN students. No portion of this
document may be reproduced, copied or revised without written
permission of the authors.

Author: Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>

"""


class City:

    GRID_LENGTH = 7
    GRID_SIZE = GRID_LENGTH * GRID_LENGTH
    WHITE = (255, 255, 255)
    POSITIONS = {}
    BUILDINGS = {}
    
    def __init__(self):
        self.grid = [
            [Cell(j, i, self.GRID_LENGTH) for i in range(self.GRID_LENGTH)]
            for j in range(self.GRID_LENGTH)
        ]
        self.font = pygame.font.Font(None, 74)
        self.turn = -1

    def updateTurn(self, turn):
        if self.turn != turn:
            City.POSITIONS = {
                (i, j): [(k, l) for k in range(4) for l in range(4)]
                for i in range(self.GRID_LENGTH)
                for j in range(self.GRID_LENGTH)
            }
            City.BUILDINGS = {}
            self.turn = turn
            return True
        return False

    def updateCell(self, row, column, typeOfCell, contaminationRate, numberOfPeople):
        self.grid[row][column].update(typeOfCell, contaminationRate, numberOfPeople)

    def draw(self, surface):
        for row in self.grid:
            for cell in row:
                cell.draw(surface)

    def isCellBuilding(self, rect, position):
        center_x = rect.x + rect.width // 2
        center_y = rect.y + rect.height // 2
        x = position % City.GRID_LENGTH
        y = position // City.GRID_LENGTH
        i, j = int(center_y // Cell.TILE), int(center_x // Cell.TILE)
        return self.grid[i][j].isBuilding() and self.grid[y][x].isBuilding()

    def getTextDay(self):
        return f"Day : {self.turn}"

    def __str__(self):
        string = ""
        for row in range(self.GRID_LENGTH):
            for column in range(self.GRID_LENGTH):
                string += str(self.grid[row][column]) + " "
            string += "\n"
        return string
                
