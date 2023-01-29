#include "headers/ata.h"
#include "headers/io.h"
#include "headers/pci.h"

// #include "headers/colors.h"
// #include "headers/screen.h"
// #include "headers/string.h"
// #include "headers/stdio.h"
// #include "headers/commands.h"
// #include "headers/fat32.h"

// Chat von OpenGPT zum Thema ATA
// Ohne Zugriff auf Standardbibliotheken oder Betriebssystemfunktionen wäre es schwierig, auf ATA-Geräte zuzugreifen und Informationen darüber abzurufen. Um direkt auf die Hardware-Schnittstelle des Computers zuzugreifen und Befehle und Daten an das ATA-Gerät zu senden, müssten Sie tiefes Wissen über die Hardware-Schnittstelle des Computers und das ATA-Protokoll haben. Hier sind einige Schritte, die Sie unternehmen könnten, um auf diese Weise auf ATA-Geräte zuzugreifen:
// Identifizieren Sie die Hardware-Schnittstelle, die verwendet wird, um auf das ATA-Gerät zuzugreifen. Dies kann entweder eine parallele oder serielle Schnittstelle sein.
// Erstellen Sie ein Programm, das die Hardware-Schnittstelle direkt ansteuert und Befehle und Daten an das ATA-Gerät sendet. Dazu müssen Sie das ATA-Protokoll verstehen und in der Lage sein, die entsprechenden Befehle und Datenformate zu generieren.
// Implementieren Sie Treiber und Unterstützung für das ATA-Gerät selbst. Dazu müssen Sie wissen, wie das ATA-Gerät funktioniert und wie es von der Hardware-Schnittstelle angesteuert wird.
// Testen und debuggen Sie Ihr Programm, um sicherzustellen, dass es richtig funktioniert und auf das ATA-Gerät zugreifen und Informationen darüber abrufen kann.
// Es ist wichtig zu beachten, dass diese Vorgehensweise sehr schwierig ist und erfordert tiefes Wissen über die Hardware-Schnittstelle des Computers und das ATA-Protokoll. Es wäre in der Regel viel einfacher und zuverlässiger, die vorhandenen Betriebssystemfunktionen und Standardbibliotheken zu verwenden, um auf ATA-Geräte zuzugreifen und Informationen darüber abzurufen.


// andere methode zum testen
static ATA_t *ATADevices;
static uint8_t *count;

void ATA_setup_static(uint32_t address_ATA_Devices, uint32_t address_count)
{
    ATADevices = (ATA_t*)address_ATA_Devices;
    count = (uint8_t*)address_count;
}

void ATA_Init(ATA_t **ptr_ATADevices, uint8_t **ptr_count)
{
    *count = 0;
    ata_detect_addresses();
    *ptr_ATADevices = ATADevices;
    *ptr_count = count;
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
    while ((x & 0x08) != 0)
    {
        x = inb(port + 7);
        if ((x & 0x01) != 0)
        {
            break;
        }
    }
    if (x == 0 || x == 1)
    {
        return;
    }
    uint16_t returned[256];
    for (uint16_t i = 0; i < 256; i++)
    {
        returned[i] = inw(port);
    }
    ATADevices[*count].port = port;
    ATADevices[*count].slavebit = slavebit;
    if (((returned[83] & 1) << 10) != 0)
    {
        ATADevices[*count].isLBA48Supported = 1;
        ATADevices[*count].LBA48 = (((uint64_t)returned[103]) << 48) + (((uint64_t)returned[102]) << 32) + (((uint64_t)returned[101]) << 16) + ((uint64_t)returned[100]);
    }
    else
    {
        ATADevices[*count].isLBA48Supported = 0;
        ATADevices[*count].LBA28 = (((uint64_t)returned[61]) << 16) + ((uint64_t)returned[60]);
        if (ATADevices[*count].LBA28 == 0)
        {
            return;
        }
    }
    *count += 1;
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

void ata_detect_addresses()
{
    pci_dev_t devices[PCI_DEV_COUNT];
    pci_init(devices);
    // uint8_t CHARARRAYMAX = 80;
    // uint8_t outputint[CHARARRAYMAX];
    // uint8_t _lastindex;
    for (uint32_t i = 0; i < PCI_DEV_COUNT; i++)
    {
        pci_dev_t pci_dev = devices[i];
        if (pci_dev.ClassCode == 0x01 && pci_dev.Subclass == 0x06 && pci_dev.ProgIF == 0x01)
        {
            // uint32_t value = pci_ConfigReadDword(pci_dev.bus, pci_dev.device, pci_dev.function, 0xA8);
            // clearArray(outputint, CHARARRAYMAX, 0x00);
            // strapp(outputint, "AHCI Devices: ", 0, CHARARRAYMAX);
            // _lastindex = lastIndex(outputint, CHARARRAYMAX);
            // ConvertToChar(((value >> 16) & 0xFFFF), 16, outputint, _lastindex);
            // _lastindex = lastIndex(outputint, CHARARRAYMAX);
            // ConvertToChar(((value >>  0) & 0xFFFF), 16, outputint, _lastindex);
            // printString(outputint, White, Black);
            // setcursornewline();
            continue;
        }
        if (pci_dev.ClassCode != 0x01 || pci_dev.Subclass != 0x01)
        {
            continue;
        }
        uint16_t address1_IO = 0, address2_IO = 0;
        // uint16_t address1_CTRL = 0, address2_CTRL = 0;
        pci_baddress_t baddr;
        pci_getBaseAddress(pci_dev.bus, pci_dev.device, pci_dev.function, &baddr);
        if (baddr.BaseAddress0 == 0x0 || baddr.BaseAddress0 == 0x1)
        {
            address1_IO = 0x1F0;
        }
        else
        {
            address1_IO = (baddr.BaseAddress0 & 0xFFFF);
        }
        if (baddr.BaseAddress2 == 0x0 || baddr.BaseAddress2 == 0x1)
        {
            address2_IO = 0x170;
        }
        else
        {
            address2_IO = (baddr.BaseAddress2 & 0xFFFF);
        }
        // if (baddr.BaseAddress1 == 0x0 || baddr.BaseAddress1 == 0x1)
        // {
        //     address1_CTRL = 0x3F4;
        // }
        // else
        // {
        //     address1_CTRL = (baddr.BaseAddress1 & 0xFFFF);
        // }
        // if (baddr.BaseAddress3 == 0x0 || baddr.BaseAddress3 == 0x1)
        // {
        //     address2_CTRL = 0x374;
        // }
        // else
        // {
        //     address2_CTRL = (baddr.BaseAddress3 & 0xFFFF);
        // }
        ATA_Detect_Devtype(address1_IO, 0);
        ATA_Detect_Devtype(address1_IO, 1);
        ATA_Detect_Devtype(address2_IO, 0);
        ATA_Detect_Devtype(address2_IO, 1);
    }
}
