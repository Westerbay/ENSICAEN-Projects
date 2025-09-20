from cell import Cell
from press import PressAgency
from citizen import CitizenManager, Citizen
from sprite import TextBox, Button

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

class Info:

    ICON_SIZE = (80, 80)    
    BLACK = (0, 0, 0)
    INFO_PATH = "resources/img/information.png"
    BUBLE_POSITION = (1500, 560)
    BUBLE_SIZE = (64, 64)

    def __init__(self):
        self.font = pygame.font.Font(TextBox.FONT, 25)
        x, y = PressAgency.GRAPH_POSITION
        self.rect = pygame.Rect(x, y, 640, 480)
        self.image = self.generateImage()
        infoBubble = pygame.image.load(self.INFO_PATH).convert_alpha()
        self.infoBubble = pygame.transform.scale(infoBubble, self.BUBLE_SIZE)
        self.infoRect = pygame.Rect(self.BUBLE_POSITION[0], self.BUBLE_POSITION[1],
                                self.BUBLE_SIZE[0], self.BUBLE_SIZE[1])

    def placeCitizens(self, surface):
        icon = pygame.image.load(f"{CitizenManager.CITIZEN_FOLDER}/icon.png").convert_alpha()
        icon = pygame.transform.scale(icon, self.ICON_SIZE)
        surface.blit(icon, (350, 10))
        icon = pygame.image.load(f"{CitizenManager.DOCTOR_FOLDER}/icon.png").convert_alpha()
        icon = pygame.transform.scale(icon, self.ICON_SIZE)
        surface.blit(icon, (350, 96))
        icon = pygame.image.load(f"{CitizenManager.FIREMAN_FOLDER}/icon.png").convert_alpha()
        icon = pygame.transform.scale(icon, self.ICON_SIZE)
        surface.blit(icon, (350, 192))
        icon = pygame.image.load(f"{CitizenManager.REPORTER_FOLDER}/icon.png").convert_alpha()
        icon = pygame.transform.scale(icon, self.ICON_SIZE)
        surface.blit(icon, (350, 288))
        icon = pygame.image.load(f"{Citizen.DEAD_PATH}").convert_alpha()
        icon = pygame.transform.scale(icon, self.ICON_SIZE)
        surface.blit(icon, (350, 384))
        return surface

    def placeTextCitizens(self, surface):
        description = self.font.render("Citizen", True, self.BLACK)
        surface.blit(description, (450, 40))
        description = self.font.render("Doctor", True, self.BLACK)
        surface.blit(description, (450, 135))
        description = self.font.render("Fireman", True, self.BLACK)
        surface.blit(description, (450, 222))
        description = self.font.render("Reporter", True, self.BLACK)
        surface.blit(description, (450, 318))
        description = self.font.render("Dead", True, self.BLACK)
        surface.blit(description, (450, 414))
        return surface

    def placeCityElements(self, surface):
        icon = pygame.image.load(f"{Cell.HEALTHY_TILE}").convert_alpha()
        icon = pygame.transform.scale(icon, self.ICON_SIZE)
        surface.blit(icon, (20, 10))
        icon = pygame.image.load(f"{Cell.HOUSE}").convert_alpha()
        icon = pygame.transform.scale(icon, self.ICON_SIZE)
        surface.blit(icon, (20, 96))
        icon = pygame.image.load(f"{Cell.HOSPITAL}").convert_alpha()
        icon = pygame.transform.scale(icon, self.ICON_SIZE)
        surface.blit(icon, (20, 192))
        icon = pygame.image.load(f"{Cell.FIRESTATION}").convert_alpha()
        icon = pygame.transform.scale(icon, self.ICON_SIZE)
        surface.blit(icon, (20, 288))
        icon = pygame.image.load(f"{Cell.SICK_TILE}").convert_alpha()
        icon = pygame.transform.scale(icon, self.ICON_SIZE)
        surface.blit(icon, (20, 384))
        return surface

    def placeTextCityElements(self, surface):
        description = self.font.render(f"Rate under {int(Cell.SICK_RATE * 100)}%", True, self.BLACK)
        surface.blit(description, (120, 40))
        description = self.font.render("House", True, self.BLACK)
        surface.blit(description, (120, 126))
        description = self.font.render("Hospital", True, self.BLACK)
        surface.blit(description, (120, 222))
        description = self.font.render("Fire Station", True, self.BLACK)
        surface.blit(description, (120, 318))
        description = self.font.render(f"Rate above {int(Cell.SICK_RATE * 100)}%", True, self.BLACK)
        surface.blit(description, (120, 414))
        return surface

    def generateImage(self):        
        surface = pygame.Surface((self.rect.width, self.rect.height))
        surface.fill((1, 22, 3))
        surface.set_colorkey((1, 22, 3))
        pygame.draw.rect(surface, PressAgency.WHITE, (0, 0, self.rect.width, self.rect.height),
                         border_radius = 50)
        surface = self.placeCitizens(surface)
        surface = self.placeCityElements(surface)
        surface = self.placeTextCitizens(surface)
        surface = self.placeTextCityElements(surface)
        return surface

    def triggered(self):
        mousePos = pygame.mouse.get_pos()
        if self.infoRect.collidepoint(mousePos):
            Button.HAND = True
            return True
        return False

    def draw(self, surface):
        surface.blit(self.infoBubble, self.BUBLE_POSITION)
        if self.triggered():
            surface.blit(self.image, self.rect)
