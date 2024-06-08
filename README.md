[![Discord][discord-shield]][discord-url]
[![Donalerts][donalerts-shield]][donalerts-url]
[![GPL License][license-shield]][license-url]

<br />
<div align="center">
  <a href="https://github.com/Erruar/RTSS-CLI">
    <img src="RTSS-CLI/Images/WindowIcon.ico" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Saku RTSS CLI</h3>

  <p align="center">
    An awesome laptop overclock utility for those who want real performance!
    <br />
    <a href="https://github.com/Erruar/RTSS-CLI/releases"><strong>Explore the releases »</strong></a>
    <br />  
    <a href="https://github.com/Erruar/RTSS-CLI/issues">Report Bug</a>
    ·
    <a href="https://github.com/Erruar/RTSS-CLI/issues">Request Feature</a>
  </p>
</div>


## About The Project

RTSS CLI is a command-line application designed to simplify the process of changing text in the On-Screen Display (OSD) of Riva Tuner Statistics Server (RTSS). The RTSS API is originally written in C++, making it extremely difficult or even impossible to integrate into projects that use other programming languages. This application bridges that gap, enabling developers to easily add support for displaying their metrics during gameplay using RTSS, regardless of their project's programming language.

## Advantages

- **Ease of Use:** Setting custom text in RTSS is simple with this application!
- **Portability:** The app is portable and can be used without any installation.
- **Customization:** You can set the position of your text, its color, and font sizes.
- **One-Command Reset:** Clear or reset the OSD with a single command.
- **Resource Efficient:** After making changes, the application exits, ensuring no more RAM or CPU usage.
- **Language Agnostic:** Easy to integrate into any programming language project.

## Getting Started

### Prerequisites

- Windows Operating System
- Visual Studio with MSVC (Microsoft Visual C++) compiler

**Note:** This project is primarily designed to run on Windows in Visual Studio with MSVC. It will not run in CLion due to differences in compilers.

### Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/RTSS-CLI.git
    ```
2. Open the project in Visual Studio.
3. Build the solution.

### Usage

Run the application from the command line with the necessary arguments to customize the RTSS OSD. Here are some example commands:

- **Set custom text, position, color:**
    ```bash
    RTSS_CLI.exe --text "Your custom text"
    ``` 
- **Clear OSD:**
    ```bash
    RTSS_CLI.exe --reset
    ```

### Integration

Integrate the RTSS CLI into your project by invoking the executable with the appropriate arguments from within your code. This makes it easy to add RTSS support to any application, regardless of the programming language used.

### License

This project is licensed under the GPL-3 License. See the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! 

## Acknowledgements

Special thanks to the developers of Riva Tuner Statistics Server for providing a robust API for OSD.

---

Feel free to copy, modify, and distribute this project in accordance with the terms of the license. Enjoy using RTSS CLI for all your OSD customization needs!

[discord-shield]: https://img.shields.io/badge/Join%20our-discord-%23ff7f50?style=for-the-badge&logo=discord&logoColor=%23ff7f50
[discord-url]: https://discord.gg/WzgsFvgTuh
[donalerts-shield]: https://img.shields.io/badge/Support%20me-DonAlerts-%23f13a13?style=for-the-badge&logo=disqus&logoColor=%23f13a13
[donalerts-url]: https://www.donationalerts.com/r/RubyTrack
[license-shield]: https://img.shields.io/badge/LICENSE%20-GPL-%230ff99C?style=for-the-badge
[license-url]: https://github.com/Erruar/RTSS-CLI/blob/master/LICENSE.md
