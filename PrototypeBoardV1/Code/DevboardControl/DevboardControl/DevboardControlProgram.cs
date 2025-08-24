using LibUsbDotNet;
using LibUsbDotNet.Main;
using System.Runtime.InteropServices;

public class DevboardControlProgram
{
    public static void Main(string[] args)
    {
        if (args == null || args.Length <= 0)
        {
            Console.WriteLine("Usage:\n DevboardControl.exe on\n DevboardControl.exe off");
            return;
        }

        var request = (byte) (args[0].ToLower() == "on" ? 1 : 0);

        UsbDevice devboard;

        int vendorId = 0x0039;
        int productId = 0x0001;

        devboard = UsbDevice.OpenUsbDevice(new UsbDeviceFinder(vendorId, productId));

        if (devboard == null)
        {
            Console.WriteLine("Device Not Found.");
            return;
        }

        try
        {
            byte[] buffer = new byte[0];
           
            int bytesRead;
            var packet = new UsbSetupPacket(
                (byte)UsbEndpointDirection.EndpointIn | (byte)UsbRequestType.TypeStandard | (byte)UsbRequestRecipient.RecipDevice | (byte)UsbRequestType.TypeVendor, 
                request, 
                0x0,
                0x0,
                0x0
            );

            var result = devboard.ControlTransfer(ref packet, buffer, buffer.Length, out bytesRead);
            if (!result)
            {
                Console.WriteLine("Error during USB interaction");
            }
        }
        finally
        {
            if (devboard != null)
            {
                devboard.Close();
            }
        }
    }
}