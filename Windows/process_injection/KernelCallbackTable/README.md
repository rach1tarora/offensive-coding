# Windows Process injection via `KernelCallbackTable`

This method of process injection was used by [FinFisher/FinSpy](https://www.microsoft.com/security/blog/2018/03/01/finfisher-exposed-a-researchers-tale-of-defeating-traps-tricks-and-complex-virtual-machines/) and [Lazarus](https://blog.malwarebytes.com/threat-intelligence/2022/01/north-koreas-lazarus-apt-leverages-windows-update-client-github-in-latest-campaign/).

The `kernelcallbacktable` is always configured when there are UI-related elements linked to the application and it is initialized to an array of functions when user32.dll is loaded into a GUI process. It is utilized to manage all pointers and structures involved in sending messages within a process, such as interprocess communication. This allows for messages to be sent between applications via the kernel callback.

The functions are invoked to perform various operations usually in response to window messages. For example, `_fnCOPYDATA` is executed in response to the `WM_COPYDATA` message, so in the PoC, this function is replaced to demonstrate the injection. Finfisher uses the `_fnDWORD` function

We simply duplicate the existing table or overwrite the address with our own payload in the kernelcallbacktable, set the `fnCOPYDATA` function to address of payload, update the PEB with address of new table and invoke using `WM_COPYDATA`. By hooking into that table, we can define our own triggers, enabling us to send messages to the application, which will in turn execute our code.


As this is a proof of concept, Explorer might crash. It's not recommended to run this on your machine; use a VM instead.

