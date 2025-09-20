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

class TextBox:

    WHITE = (255, 255, 255)
    BLACK = (0, 0, 0)
    FONT = "resources/font.ttf"

    def __init__(self, rect, getText, fontsize = 40):
        self.getText = getText
        self.rect = rect
        self.font = pygame.font.Font(self.FONT, fontsize)
        self.width = rect.width
        self.height = rect.height

    def draw(self, surface):
        image = pygame.Surface((self.width, self.height))
        image.fill((1, 2, 3))
        image.set_colorkey((1, 2, 3))
        text = self.font.render(self.getText(), False, self.BLACK)
        pygame.draw.rect(image, self.WHITE, (0, 0, self.width, self.height), border_radius = 20)
        width, height = text.get_size()
        image.blit(text, ((self.width - width) // 2, (self.height - height) // 2))
        surface.blit(image, self.rect)

class Button(TextBox):
    
    HAND = False

    def __init__(self, rect, getText, action, getColor, fontsize = 30):
        TextBox.__init__(self, rect, getText, fontsize)
        self.action = action
        self.getColor = getColor

    def draw(self, surface):
        self.update()
        image = pygame.Surface((self.width, self.height))
        image.fill((1, 2, 3))
        image.set_colorkey((1, 2, 3))
        text = self.font.render(self.getText(), False, self.WHITE)
        pygame.draw.rect(image, self.getColor(), (0, 0, self.width, self.height))
        pygame.draw.rect(image, self.BLACK, (0, 0, self.width, self.height), 5)
        width, height = text.get_size()
        image.blit(text, ((self.width - width) // 2, (self.height - height) // 2))
        surface.blit(image, self.rect)

    def update(self):
        mousePos = pygame.mouse.get_pos()
        if self.rect.collidepoint(mousePos):
            Button.HAND = True            
            
    def onClick(self):
        mousePos = pygame.mouse.get_pos()
        if self.rect.collidepoint(mousePos):
            self.action()
            


    
