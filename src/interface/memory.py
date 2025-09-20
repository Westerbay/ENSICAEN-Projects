from city import City
from citizen import CitizenManager

import mmap
import posix_ipc
import struct


"""

ENSICAEN
6 Boulevard Maréchal Juin
F-14050 Caen Cedex
 
This file is owned by ENSICAEN students. No portion of this
document may be reproduced, copied or revised without written
permission of the authors.

Author: Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>

"""


class MemoryShared:

    SHM_NAME = "/os_project_memory"
    
    cell_format = "IIddII"
    
    citizen_format = "IIIIIIdddd"
    fireman_format = f"{citizen_format}"
    doctor_format = f"{citizen_format}"
    reporter_format = f"{citizen_format}"
    
    citizenlist_format = citizen_format * CitizenManager.NUMBER_CITIZEN
    firemenlist_format = fireman_format * CitizenManager.NUMBER_FIREMAN
    doctorlist_format = doctor_format * CitizenManager.NUMBER_DOCTOR    
    reporterlist_format = reporter_format * CitizenManager.NUMBER_REPORTER
    
    grid_format = cell_format * City.GRID_SIZE
    FORMAT = f"IIIIII{grid_format + firemenlist_format + doctorlist_format + citizenlist_format + reporterlist_format}"
    
    def __init__(self):
        self.memoryShared = posix_ipc.SharedMemory(self.SHM_NAME)
        self.size = struct.calcsize(self.FORMAT)
        self.gridSize = len(self.grid_format)
        self.citizenSize = len(self.citizenlist_format)
        self.firemenSize = len(self.firemenlist_format)
        self.doctorSize = len(self.doctorlist_format)
        self.reporterSize = len(self.reporterlist_format)
        self.fd = self.memoryShared.fd

    def update(self, interface, city, citizenManager):
        with mmap.mmap(self.fd, self.size) as mem:
            mem.seek(0)
            raw_data = mem.read(self.size)
            data = struct.unpack(self.FORMAT, raw_data)
            
            interface.pid = [data[1], data[2], data[3], data[4]]
            turn = data[5]
            size = 6
            
            cells = data[size:self.gridSize + size]
            size += self.gridSize

            firemen = data[size:self.firemenSize + size]
            size += self.firemenSize

            doctors = data[size:self.doctorSize + size]
            size += self.doctorSize

            reporters = data[size:self.reporterSize + size]
            size += self.reporterSize

            citizens = data[size:self.citizenSize + size]
            size += self.citizenSize       
            
            if city.updateTurn(turn):
                self.updateCells(city, cells)
                self.updateCitizens(citizenManager, citizens, firemen, doctors, reporters)

    def updateCitizens(self, citizenManager, citizens, firemen, doctors, reporters):

        for i in range(citizenManager.NUMBER_CITIZEN):
            position = citizens[i * len(self.citizen_format)]
            state = citizens[i * len(self.citizen_format) + 1]
            citizenManager.citizens[i].reload(position, state)

        for i in range(citizenManager.NUMBER_DOCTOR):
            position = doctors[i * len(self.doctor_format)]
            state = doctors[i * len(self.doctor_format) + 1]
            citizenManager.doctors[i].reload(position, state)

        for i in range(citizenManager.NUMBER_FIREMAN):
            position = firemen[i * len(self.fireman_format)]
            state = firemen[i * len(self.fireman_format) + 1]
            citizenManager.firemen[i].reload(position, state)

        for i in range(citizenManager.NUMBER_REPORTER):
            position = reporters[i * len(self.reporter_format)]
            state = reporters[i * len(self.reporter_format) + 1]
            citizenManager.reporters[i].reload(position, state)

    def writeEndOfSimulation(self):
         with mmap.mmap(self.fd, self.size) as mem:
            mem.seek(0)
            mem.write(struct.pack('I', 1))

    def updateCells(self, city, cells):
        for row in range(city.GRID_LENGTH):
            for column in range(city.GRID_LENGTH):
                index = (row * city.GRID_LENGTH + column) * len(self.cell_format)
                typeOfCell = cells[index]
                contaminationRate = cells[index + 2]
                numberOfPeople = cells[index + 4]
                city.updateCell(row, column, typeOfCell, contaminationRate, numberOfPeople)
