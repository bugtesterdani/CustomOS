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

channel_t channels[2];
ide_device_t devices[4];
uint8_t ide_buffer[2048];

void InitializeATA(channel_t channel[2], ide_device_t device[4], uint8_t ide_buff[2048])
{
    // get IDE Devices
    ata_GetIDE();
    // get SATA Devices
    ata_GetSATA();
    for (uint8_t i = 0; i < 2; i++)
    {
        channel[i] = channels[i];
    }
    for (uint8_t i = 0; i < 4; i++)
    {
        device[i] = devices[i];
    }
    for (uint16_t i = 0; i < 2048; i++)
    {
        ide_buff[i] = ide_buffer[i];
    }
}

void ata_GetIDE()
{
    pci_dev_t devices[PCI_DEV_COUNT];
    pci_init(devices);
    for (uint8_t i = 0; i < PCI_DEV_COUNT; i++)
    {
        pci_dev_t dev = devices[i];
        if (dev.vendorID == 0 && dev.deviceID == 0)
        {
            return;
        }
        ata_checkDevice(&dev);
    }
}

void ata_GetSATA()
{

}

void ata_checkDevice(pci_dev_t *dev)
{
    pci_baddress_t baseaddresses;
    // Not a Mass Storage Device
    if ((*dev).ClassCode != 0x01)
    {
        return;
    }
    // No implementation for this whole Part
    if ((*dev).HeaderType != 0x00)
    {
        return;
    }
    pci_getBaseAddresses(dev, &baseaddresses);
    if (baseaddresses.BaseAddress0 <= 0x01)
    {
        baseaddresses.BaseAddress0 = 0x1F0;
    }
    if (baseaddresses.BaseAddress1 <= 0x01)
    {
        baseaddresses.BaseAddress1 = 0x3F6;
    }
    if (baseaddresses.BaseAddress2 <= 0x01)
    {
        baseaddresses.BaseAddress2 = 0x170;
    }
    if (baseaddresses.BaseAddress3 <= 0x01)
    {
        baseaddresses.BaseAddress3 = 0x376;
    }
    ata_ide_initialize(dev, &baseaddresses);
}

void ata_ide_initialize(pci_dev_t *dev, pci_baddress_t *baddress)
{
    channels[ATA_PRIMARY  ].base  = ((*baddress).BaseAddress0 & 0xFFFFFFFC) + 0x1F0 * (!(*baddress).BaseAddress0);
    channels[ATA_PRIMARY  ].ctrl  = ((*baddress).BaseAddress1 & 0xFFFFFFFC) + 0x3F6 * (!(*baddress).BaseAddress1);
    channels[ATA_SECONDARY].base  = ((*baddress).BaseAddress2 & 0xFFFFFFFC) + 0x170 * (!(*baddress).BaseAddress2);
    channels[ATA_SECONDARY].ctrl  = ((*baddress).BaseAddress3 & 0xFFFFFFFC) + 0x376 * (!(*baddress).BaseAddress3);
    channels[ATA_PRIMARY  ].bmide = ((*baddress).BaseAddress4 & 0xFFFFFFFC) + 0;
    channels[ATA_SECONDARY].bmide = ((*baddress).BaseAddress4 & 0xFFFFFFFC) + 8;

    ata_ide_write(ATA_PRIMARY  , ATA_REG_CONTROL, 2);
    ata_ide_write(ATA_SECONDARY, ATA_REG_CONTROL, 2);
    
    #define IMax 2
    
    for (uint8_t i = 0; i < IMax; i++)
    {
        for (uint8_t j = 0; j < 2; j++)
        {
            ata_parseIDEDevice(i, j, IMax);
        }
    }
}

void ata_parseIDEDevice(uint8_t i, uint8_t j, uint8_t imax)
{
    uint8_t count = (i * 2 + j);
    devices[count].reseerved = 0;
    ata_ide_write(i, ATA_REG_HDDEVSEL, 0xA0 | (j << 4));
    ata_ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
    if (ata_ide_read(i, ATA_REG_STATUS) == 0)
    {
        return;
    }
    uint8_t status, err = 2;
    while (err == 2)
    {
        status = ata_ide_read(i, ATA_REG_STATUS);
        if ((status & ATA_SR_ERR))
        {
            err = 1;
        }
        if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ))
        {
            err = 0;
        }
    }

    ata_ide_read_buffer(i, ATA_REG_DATA, (uint32_t) ide_buffer, 128);
    devices[count].reseerved = 1;
    devices[count].type = IDE_ATA;
    devices[count].channel = i;
    devices[count].drive = j;
    devices[count].sign = *((uint16_t *)(ide_buffer + ATA_IDENT_DEVICETYPE));
    devices[count].capabilities = *((uint16_t *)(ide_buffer + ATA_IDENT_CAPABILITIES));
    devices[count].commandsets = *((uint32_t *)(ide_buffer + ATA_IDENT_COMMANDSETS));
    if (devices[count].commandsets & (1 << 26))
    {
        devices[count].size = *((uint32_t *)(ide_buffer + ATA_IDENT_MAX_LBA_EXT));
    }
    else
    {
        devices[count].size = *((uint32_t *)(ide_buffer + ATA_IDENT_MAX_LBA));
    }
}

void ata_400nsdelay()
{
    for (uint8_t i = 0; i < 15; i++)
    {
        ata_ide_read(0, 0);
    }
}

void ata_cache_flush_write(uint8_t channel, uint8_t reg)
{
    ata_ide_write(channel, reg, 0xE7);
}

void ata_ide_write(uint8_t channel, uint8_t reg, uint8_t data)
{
    if (reg > 0x07 && reg < 0x0C)
    {
        ata_ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
    }
    if (reg < 0x08)
    {
        outb(data, channels[channel].base  + reg - 0x00);
    }
    else if (reg < 0x0C)
    {
        outb(data, channels[channel].base  + reg - 0x06);
    }
    else if (reg < 0x0E)
    {
        outb(data, channels[channel].ctrl  + reg - 0x0A);
    }
    else if (reg < 0x16)
    {
        outb(data, channels[channel].bmide + reg - 0x0E);
    }
    if (reg > 0x07 && reg < 0x0C)
    {
        ata_ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
    }
}

uint8_t ata_ide_read(uint8_t channel, uint8_t reg)
{
    uint8_t result;
    if (reg > 0x07 && reg < 0x0C)
    {
        ata_ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
    }
    if (reg < 0x08)
    {
        inb(channels[channel].base  + reg - 0x00);
    }
    else if (reg < 0x0C)
    {
        inb(channels[channel].base  + reg - 0x06);
    }
    else if (reg < 0x0E)
    {
        inb(channels[channel].ctrl  + reg - 0x0A);
    }
    else if (reg < 0x16)
    {
        inb(channels[channel].bmide + reg - 0x0E);
    }
    if (reg > 0x07 && reg < 0x0C)
    {
        ata_ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
    }
}

void ata_ide_read_buffer(uint8_t channel, uint8_t reg, uint32_t buffer, uint32_t quads)
{
    if (reg > 0x07 && reg < 0x0C)
    {
        ata_ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
    }
    
    __asm__ __volatile__ ("pushw %es");
    __asm__ __volatile__ ("movw %ds, %ax");
    __asm__ __volatile__ ("movw %ax, %es");
    __asm__ __volatile__ ("popw %es");

    if (reg > 0x07 && reg < 0x0C)
    {
        ata_ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
    }
}
