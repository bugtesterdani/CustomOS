#include "headers/ata.h"
#include "headers/io.h"

// Chat von OpenGPT zum Thema ATA
// Ohne Zugriff auf Standardbibliotheken oder Betriebssystemfunktionen wäre es schwierig, auf ATA-Geräte zuzugreifen und Informationen darüber abzurufen. Um direkt auf die Hardware-Schnittstelle des Computers zuzugreifen und Befehle und Daten an das ATA-Gerät zu senden, müssten Sie tiefes Wissen über die Hardware-Schnittstelle des Computers und das ATA-Protokoll haben. Hier sind einige Schritte, die Sie unternehmen könnten, um auf diese Weise auf ATA-Geräte zuzugreifen:
// Identifizieren Sie die Hardware-Schnittstelle, die verwendet wird, um auf das ATA-Gerät zuzugreifen. Dies kann entweder eine parallele oder serielle Schnittstelle sein.
// Erstellen Sie ein Programm, das die Hardware-Schnittstelle direkt ansteuert und Befehle und Daten an das ATA-Gerät sendet. Dazu müssen Sie das ATA-Protokoll verstehen und in der Lage sein, die entsprechenden Befehle und Datenformate zu generieren.
// Implementieren Sie Treiber und Unterstützung für das ATA-Gerät selbst. Dazu müssen Sie wissen, wie das ATA-Gerät funktioniert und wie es von der Hardware-Schnittstelle angesteuert wird.
// Testen und debuggen Sie Ihr Programm, um sicherzustellen, dass es richtig funktioniert und auf das ATA-Gerät zugreifen und Informationen darüber abrufen kann.
// Es ist wichtig zu beachten, dass diese Vorgehensweise sehr schwierig ist und erfordert tiefes Wissen über die Hardware-Schnittstelle des Computers und das ATA-Protokoll. Es wäre in der Regel viel einfacher und zuverlässiger, die vorhandenen Betriebssystemfunktionen und Standardbibliotheken zu verwenden, um auf ATA-Geräte zuzugreifen und Informationen darüber abzurufen.


// andere methode zum testen
uint16_t returned[256];
ATA_t ATADevices[4];
uint8_t count = 0;

void ATA_Init(ATA_t loc_ATADevices[4], uint8_t *loc_count)
{
    count = 0;
    ata_detect_devices();
    *loc_count = count;
    for (uint8_t i = 0; i < 4; i++)
    {
        loc_ATADevices[i] = ATADevices[i];
    }
}

void ATA_Detect_Devtype(uint16_t port, uint8_t slavebit)
{
    outb(port + 6, ATA_MASTER_PORT | ((slavebit & ATA_SLAVE_PORT_BIT) << 4));
    outb(port + 2, 0);
    outb(port + 3, 0);
    outb(port + 4, 0);
    outb(port + 5, 0);
    outb(port + 7, ATA_CMD_IDENTIFY);
    uint8_t x = inb(port + 7);
    while (x & 0x08 != 0)
    {
        x = inb(port + 7);
        if (x & 0x01 != 0)
        {
            break;
        }
    }
    if (x == 0 || x == 1)
    {
        return;
    }
    for (uint16_t i = 0; i < 256; i++)
    {
        returned[i] = inw(port);
    }
    ATADevices[count].port = port;
    ATADevices[count].slavebit = slavebit;
    if (returned[83] & 1 << 10 != 0)
    {
        ATADevices[count].isLBA48Supported = 1;
        ATADevices[count].LBA48 = (returned[103] << 48) + (returned[102] << 32) + (returned[101] << 16) + returned[100];
    }
    else
    {
        ATADevices[count].isLBA48Supported = 0;
        ATADevices[count].LBA28 = (returned[61] << 16) + returned[60];
        if (ATADevices[count].LBA28 == 0)
        {
            return;
        }
    }
    count++;
    return;
}

void ataRead(uint8_t drive_id, uint64_t LBA, uint8_t cnt, void *addr)
{
    if (ATADevices[drive_id].isLBA48Supported == 1)
    {
        ataRead48(drive_id, LBA, cnt, addr);
    }
    else
    {
        ataRead28(drive_id, LBA, cnt, addr);
    }
}

void ataWrite(uint8_t drive_id, uint64_t LBA, uint8_t cnt, void *addr)
{
    if (ATADevices[drive_id].isLBA48Supported == 1)
    {
        ataWrite48(drive_id, LBA, cnt, addr);
    }
    else
    {
        ataWrite28(drive_id, LBA, cnt, addr);
    }
}

void ataRead28(uint8_t drive_id, uint64_t LBA, uint8_t cnt, void *addr)
{
    ATA_t device = ATADevices[drive_id];
    uint16_t port = device.port;
    uint8_t slavebit = device.slavebit;
    outb(port + 6, 0xE0 | ((slavebit << 4) & ATA_SLAVE_PORT_BIT) | ((LBA >> 24) & 0x0F));
    outb(port + 2, cnt);
    outb(port + 3, LBA & 0xFF);
    outb(port + 4, (LBA >> 8) & 0xFF);
    outb(port + 5, (LBA >> 16) & 0xFF);
    outb(port + 7, ATA_CMD_READ_PIO);
    uint16_t dcr = 0x3F6;
    if (port == 0x170)
    {
        dcr = 0x376;
    }
    uint8_t x = inb(port + 7);
    x = inb(port + 7);
    x = inb(port + 7);
    x = inb(port + 7);
    while (x & 0x80)
    {
        x = inb(port + 7);
    }
    for (uint32_t i = 0; i < 256 * cnt; i++)
    {
        uint16_t h = inw(port);
        *((uint8_t*)addr + (i * 2)) = (h & 0xFF);
        *((uint8_t*)addr + (i * 2) + 1) = ((h >> 8) & 0xFF);
    }
}

void ataWrite28(uint8_t drive_id, uint64_t LBA, uint8_t cnt, void *addr)
{
    ATA_t device = ATADevices[drive_id];
    uint16_t port = device.port;
    uint8_t slavebit = device.slavebit;
    outb(port + 6, 0xE0 | ((slavebit << 4) & ATA_SLAVE_PORT_BIT) | ((LBA >> 24) & 0x0F));
    outb(port + 2, cnt);
    outb(port + 3, LBA & 0xFF);
    outb(port + 4, (LBA >> 8) & 0xFF);
    outb(port + 5, (LBA >> 16) & 0xFF);
    outb(port + 7, ATA_CMD_WRITE_PIO);
    uint16_t dcr = 0x3F6;
    if (port == 0x170)
    {
        dcr = 0x376;
    }
    uint8_t x = inb(port + 7);
    x = inb(port + 7);
    x = inb(port + 7);
    x = inb(port + 7);
    while (x & 0x80)
    {
        x = inb(port + 7);
    }
    for (uint32_t i = 0; i < 256 * cnt; i++)
    {
        outw(port, ((*((uint8_t*)addr + i * 2 + 1)) << 8) + (*((uint8_t*)addr + i * 2)));
        outb(port + 7, ATA_CMD_CACHE_FLUSH);
        x = inb(port + 7);
        while (x & 0x80)
        {
            x = inb(port + 7);
        }
    }
}

void ataWrite48(uint8_t drive_id, uint64_t LBA, uint8_t cnt, void *addr)
{
    ATA_t device = ATADevices[drive_id];
    uint16_t port = device.port;
    uint8_t slavebit = device.slavebit;
    outb(port + 6, 0x40 | ((slavebit << 4) & ATA_SLAVE_PORT_BIT));
    outb(port + 2, 0);
    outb(port + 3, (LBA >> 24) & 0xFF);
    outb(port + 4, (LBA >> 32) & 0xFF);
    outb(port + 5, (LBA >> 40) & 0xFF);
    outb(port + 2, cnt);
    outb(port + 3, (LBA >>  0) & 0xFF);
    outb(port + 4, (LBA >>  8) & 0xFF);
    outb(port + 5, (LBA >> 16) & 0xFF);
    outb(port + 7, ATA_CMD_WRITE_PIO_EXT);
    uint16_t dcr = 0x3F6;
    if (port == 0x170)
    {
        dcr = 0x376;
    }
    uint8_t x = inb(port + 7);
    x = inb(port + 7);
    x = inb(port + 7);
    x = inb(port + 7);
    while (x & 0x80)
    {
        x = inb(port + 7);
    }
    for (uint32_t i = 0; i < 256 * cnt; i++)
    {
        outw(port, ((*((uint8_t*)addr + i * 2 + 1)) << 8) + (*((uint8_t*)addr + i * 2)));
        outw(port + 7, ATA_CMD_CACHE_FLUSH);
        x = inb(port + 7);
        while (x & 0x80)
        {
            x = inb(port + 7);
        }
    }
}

void ataRead48(uint8_t drive_id, uint64_t LBA, uint8_t cnt, void *addr)
{
    ATA_t device = ATADevices[drive_id];
    uint16_t port = device.port;
    uint8_t slavebit = device.slavebit;
    outb(port + 6, 0x40 | ((slavebit << 4) & ATA_SLAVE_PORT_BIT));
    outb(port + 2, 0);
    outb(port + 3, (LBA >> 24) & 0xFF);
    outb(port + 4, (LBA >> 32) & 0xFF);
    outb(port + 5, (LBA >> 40) & 0xFF);
    outb(port + 2, cnt);
    outb(port + 3, (LBA >>  0) & 0xFF);
    outb(port + 4, (LBA >>  8) & 0xFF);
    outb(port + 5, (LBA >> 16) & 0xFF);
    outb(port + 7, ATA_CMD_READ_PIO_EXT);
    uint16_t dcr = 0x3F6;
    if (port == 0x170)
    {
        dcr = 0x376;
    }
    uint8_t x = inb(port + 7);
    x = inb(port + 7);
    x = inb(port + 7);
    x = inb(port + 7);
    while (x & 0x80)
    {
        x = inb(port + 7);
    }
    for (uint32_t i = 0; i < 256 * cnt; i++)
    {
        uint16_t h = inw(port);
        *((uint8_t*)addr + i*2) = (h & 0xFF);
        *((uint8_t*)addr + i*2 + 1) = ((h >> 8) & 0xFF);
    }
}

void ata_detect_devices()
{
    ATA_Detect_Devtype(0x1F0, 0);
    ATA_Detect_Devtype(0x1F0, 1);
    ATA_Detect_Devtype(0x170, 0);
    ATA_Detect_Devtype(0x170, 1);
}
