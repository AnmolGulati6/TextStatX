# TextStatX

TextStatX is a command-line text analysis tool developed in C, inspired by UNIX utilities. It provides users with comprehensive statistics about the content of text files, including the frequency of characters, words, and lines, as well as identifying the longest word and line.

## Features
- Track frequency and position of ASCII characters (0-127)
- Count words and lines, and identify unique occurrences
- Determine the longest word and line in a file
- Accept analysis requests via command-line flags or batch file

## Usage
```
./TextStatX -f <input file> -o <output file> -c -w -l -Lw -Ll
```
Or use batch mode:
```
./TextStatX -B <batch file>

## Getting Started
To get started with TextStatX, simply clone this repository and compile the source code using a C compiler. Refer to the project documentation for detailed instructions.

## Contributing
Contributions are welcome! Please fork the repository, make your changes, and submit a pull request.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
```
