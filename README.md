# CASCLibSharp

A wrapper around Ladislav Zezula's CASC Library for .NET that provides classes and methods to read Blizzard's CASC (Content Addressable Storage Container) which are used by multiple games on their platform.

# Requirements

The binaries provided require the following:

# Using the Library

Download x86/x64 Debug/Release binaries from the [Releases](https://github.com/TXC/CASCLibSharp/releases) page and add them to your .NET project (depending on if your projects are x86/x64). See the [Wiki](https://github.com/TXC/CASCLibSharp/wiki) for API documentation or see below for quick examples.

A basic [Example Project](https://github.com/TXC/CASCLibSharp/tree/main/src/CASCLibSharp.MAUIExample) in .NET MAUI Blazor Hybrid is provided that shows how the library could possibly be used in a UI.

# Reporting Bugs/Contributing

The library has been extensively tested on Overwatch, Diablo II Resurrected, Call of Duty: Modern Warfare Beta, and Call of Duty: Black Ops IIII. If you find any bugs simply open an issues on the [Issues](https://github.com/TXC/CASCLibSharp/issues) page with the required info. The library was made for my own tools but I'm happy to fix any bugs with specific games, etc. and to possibly provide more features.

The library has been written to target the .NET 8 and is only tested on Windows.

If you'd like to contribute simply fork, edit, and make a pull request. Make sure to document methods, properties, etc. Contributed code for this library is assumed to be licensed to me under the same [License](https://github.com/TXC/CASCLibSharp/blob/main/LICENSE) as the library.

# Quick Start Examples

Open a CASC Storage:

## C#

```cs
using CASCLibSharp;

// Open Overwatch
using CASCStorage storage = new ("D:\\Battle.NET\\Overwatch"));

...
```

Open a CASC Storage and print all files:

## C#

```cs
using CASCLibSharp;

// Open Black Ops 4
using CASCStorage storage = new ("D:\\Battle.NET\\Call of Duty Black Ops 4"));

Console.WriteLine("Files contained in CASC Storage: {0}", storage.FilePath);

foreach (CASCFileInfo file in storage.Files)
{
    Console.WriteLine("Name: {0}", file.FileName);
    Console.WriteLine("Size: {0}", file.FileSize);
    Console.WriteLine("Local: {0}", file.IsLocal);
    Console.WriteLine();
}

```

Open a CASC Storage and open files:

## C#

```cs
using CASCLibSharp;

// Open Black Ops 4
using CASCStorage storage = new ("D:\\Battle.NET\\Call of Duty Black Ops 4"));

// CASCStorage.OpenFile(...) returns a Stream-derived obj that can be passed to BinaryReader, etc.
foreach(CASCFileInfo file in storage.Files)
{
  using CASCFileStream reader = storage.OpenFile(file.FileName));
  
  ...
}
```

# License/Disclaimers

The library is available under the MIT License like CascLib and can be used for any purpose.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
