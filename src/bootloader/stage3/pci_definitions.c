#include "headers/pci_definitions.h"

char* pci_returnDetails(pci_dev_t *dev)
{
    switch (dev->ClassCode)
    {
        // Unclassified
        case 0x00:
            switch (dev->Subclass)
            {
                // Non_VGA_Compatible_Unclassified_Device
                case 0x00:
                    return "Unclassified device";
                // VGA_Compatible_Unclassified_Device
                case 0x01:
                    return "VGA compatible device";
                default:
                    return "Unknown device";
            }
            break;
        // MassStorageController
        case 0x01:
            switch (dev->Subclass)
            {
                // SCSI_Bus_Controller
                case 0x00:
                    return "SCSI bus controller";
                // IDEController
                case 0x01:
                    switch (dev->ProgIF)
                    {
                        // ISA_CompatibleModeOnlyController
                        case 0x00:
                            return "ISA compatible controller";
                        // PCI_nativeModeOnlyController
                        case 0x05:
                            return "PCI native mode only controller";
                        // ISA_CompatibleModeController
                        case 0x0A:
                            return "ISA compatible controller";
                        // PCI_native_mode_controller
                        case 0x0F:
                            return "PCI native mode controller";
                        // ISA_Compatibility_mode_only_controller
                        case 0x80:
                            return "ISA compatibility mode only controller";
                        // PCI_native_mode_only_controller
                        case 0x85:
                            return "PCI native mode only controller";
                        // ISA_Compatibility_mode_only_controller_master
                        case 0x8A:
                            return "ISA compatible controller master";
                        // PCI_native_mode_controller_master
                        case 0x8F:
                            return "PCI native mode controller master";
                        default:
                            return "Unknown device";
                    }
                    break;
                // FloppyDiskController
                case 0x02:
                    return "Floppy disk controller";
                // IPIController
                case 0x03:
                    return "IPI controller";
                // RAIDController
                case 0x04:
                    return "RAID controller";
                // ATAController
                case 0x05:
                    switch (dev->ProgIF)
                    {
                        // SingleDMA
                        case 0x20:
                            return "Single DMA controller";
                        // ChainedDMA
                        case 0x30:
                            return "Chained DMA controller";
                        default:
                            return "Unknown device";
                    }
                    break;
                // SATAController
                case 0x06:
                    switch (dev->ProgIF)
                    {
                        // VendorSpecificInterface
                        case 0x00:
                            return "Vendor specific interface";
                        // AHCI_10
                        case 0x01:
                            return "AHCI 10";
                        // SerialStorageBus
                        case 0x02:
                            return "Serial storage bus";
                        default:
                            return "Unknown device";
                    }
                    break;
                // Serial Attached SCSI Controller 
                case 0x07:
                    switch (dev->ProgIF)
                    {
                        // SAS
                        case 0x00:
                            return "SAS controller";
                        // Serial Controller Bus
                        case 0x01:
                            return "Serial controller bus";
                        default:
                            return "Unknown device";
                    }
                    break;
                // Non-Volatile Memory Controller
                case 0x08:
                    switch (dev->ProgIF)
                    {
                        // NVHMCI
                        case 0x00:
                            return "NVHMCI Device";
                        // NVM Express
                        case 0x01:
                            return "NVM Express Device";
                        default:
                            return "Unknown device";
                    }
                    break;
                // Other
                case 0x80:
                    return "Other Mass Storage Device";
                default:
                    return "Unknown device";
            }
            break;
        // Network Controller
        case 0x02:
            switch (dev->Subclass)
            {
                // Ethernet Controller
                case 0x00:
                    return "Ethernet controller";
                // Token Ring Controller
                case 0x01:
                    return "Token ring controller";
                // FDDI Controller
                case 0x02:
                    return "FDDI controller";
                // ATM Controller
                case 0x03:
                    return "ATM controller";
                // ISDN Controller
                case 0x04:
                    return "ISDN controller";
                // WorldFip Controller
                case 0x05:
                    return "WorldFip controller";
                // PICMG 2.14 Multi Computing Controller
                case 0x06:
                    return "PICMG 2.14 Multi Computing controller";
                // Infiniband Controller
                case 0x07:
                    return "Infiniband controller";
                // Fabric Controller
                case 0x08:
                    return "Fabric controller";
                // Other
                case 0x80:
                    return "Other Network Controller";
                default:
                    return "Unknown device";
            }
            break;
        // Display Controller
        case 0x03:
            switch (dev->Subclass)
            {
                // VGA Compatible Controller
                case 0x00:
                    switch (dev->ProgIF)
                    {
                        // VGA Controller
                        case 0x00:
                            return "VGA controller";
                        // 8514-Compatible Controller
                        case 0x01:
                            return "8514-compatible controller";
                        default:
                            return "Unknown device";
                    }
                    break;
                // XGA Controller
                case 0x01:
                    return "XGA controller";
                // 3D Controller (Not VGA-Compatible)
                case 0x02:
                    return "3D controller";
                // Other
                case 0x80:
                    return "Other Display Controller";
                default:
                    return "Unknown device";
            }
            break;
        // Multimedia Controller
        case 0x04:
            switch (dev->Subclass)
            {
                // Multimedia Video Controller
                case 0x00:
                    return "Multimedia video controller";
                // Multimedia Audio Controller
                case 0x01:
                    return "Multimedia audio controller";
                // Computer Telephony Device
                case 0x02:
                    return "Computer telephony device";
                // Audio Device
                case 0x03:
                    return "Audio device";
                // Other
                case 0x80:
                    return "Other Multimedia controller";
                default:
                    return "Unknown device";
            }
            break;
        // Memory Controller
        case 0x05:
            switch (dev->Subclass)
            {
                // RAM Controller
                case 0x00:
                    return "RAM controller";
                // Flash Controller
                case 0x01:
                    return "Flash controller";
                // Other
                case 0x80:
                    return "Other Memory Controller";
                default:
                    return "Unknown device";
            }
            break;
        // Bridge
        case 0x06:
            switch (dev->Subclass)
            {
                // Host Bridge
                case 0x00:
                    return "Host bridge";
                // ISA Bridge
                case 0x01:
                    return "ISA bridge";
                // EISA Bridge
                case 0x02:
                    return "EISA bridge";
                // MCA Bridge
                case 0x03:
                    return "MCA bridge";
                // PCI-to-PCI Bridge
                case 0x04:
                    switch (dev->ProgIF)
                    {
                        // Normal Decode
                        case 0x00:
                            return "PCI-to-PCI bridge Normal";
                        // Subtractive Decode
                        case 0x01:
                            return "PCI-to-PCI bridge Subtractive";
                        default:
                            return "Unknown device";
                    }
                    break;
                // PCMCIA Bridge
                case 0x05:
                    return "PCMCIA bridge";
                // NuBus Bridge
                case 0x06:
                    return "NuBus bridge";
                // CardBus Bridge
                case 0x07:
                    return "CardBus bridge";
                // RACEway Bridge
                case 0x08:
                    switch (dev->ProgIF)
                    {
                        // Transparent Mode
                        case 0x00:
                            return "Transparent Mode";
                        // Endpoint Mode
                        case 0x01:
                            return "Endpoint Mode";
                        default:
                            return "Unknown device";
                    }
                    break;
                // PCI-to-PCI Bridge
                case 0x09:
                    switch (dev->ProgIF)
                    {
                        // Semi-Transparent, Primary bus towards host CPU
                        case 0x40:
                            return "PCI-to-PCI bridge Semi-Transparent, Primary bus towards host CPU";
                        // Semi-Transparent, Secondary bus towards host CPU
                        case 0x80:
                            return "PCI-to-PCI bridge Semi-Transparent, Secondary bus towards host CPU";
                    }
                    break;
                // InfiniBand-to-PCI Host Bridge
                case 0x0A:
                    return "InfiniBand-to-PCI Host Bridge";
                // Other
                case 0x80:
                    return "Other Bridge";
                default:
                    return "Unknown device";
            }
        default:
            return "Not implemented String device";
        break;
    }

    return "Error happened";
}