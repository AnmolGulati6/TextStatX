#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store character information
struct CharInfo {
    char character;
    int frequency;
    int initial_position;
};
struct WordInfo {
    char *word;
    int frequency;
    int initial_position;
};
struct LineInfo {
    char *line;
    int frequency;
    int initial_position;
};
struct wordInfo {
    char *word;
    int length;
};
struct lineInfo {
    char *line;
    int length;
};

struct params {
    char *input;
    char *output;
    int c;
    int w;
    int l;
    int Lw;
    int Ll;
};

// Function to compare two characters for sorting
int compareChars(const void *a, const void *b) {
    return ((struct CharInfo*)a)->character - ((struct CharInfo*)b)->character;
}

int compareWords(const void *a, const void *b) {
    const struct wordInfo *wordA = (const struct wordInfo *)a;
    const struct wordInfo *wordB = (const struct wordInfo *)b;
    return strcmp(wordA->word, wordB->word);
}

int compareLines(const void *a, const void *b) {
    return strcmp(((struct LineInfo*)a)->line, ((struct LineInfo*)b)->line);
}

void removeDuplicates(struct wordInfo words[], int *count) {
    int i = 0;
    while (i < *count - 1) {
        if (strcmp(words[i].word, words[i + 1].word) == 0) {
            free(words[i + 1].word); // Free the duplicate word memory
            for (int j = i + 1; j < *count - 1; j++) {
                words[j] = words[j + 1]; // Shift elements to the left
            }
            (*count)--; // Decrease the count of words
        } else {
            i++; // Only move to the next word if no duplicate was found
        }
    }
}

void c(const char *inputFileName) {
    FILE *file = fopen(inputFileName, "r");
    if(file == NULL) {
        printf("ERROR: Can't open input file\n");
        exit(1);
    }
    // Initialize character information array
    struct CharInfo charInfo[256] = {0};
    int totalChars = 0;
    int totalUniqueChars = 0;
    int position = 0; // Initialize position

    // Read and process characters from the file
    int c;
    while ((c = fgetc(file)) != EOF) {
        char character = (char)c;
        totalChars++;

        if (charInfo[c].frequency == 0) {
            charInfo[c].character = character;
            charInfo[c].initial_position = position; // Update initial position
            totalUniqueChars++;
        }

        charInfo[c].frequency++;
        position++; // Increment position
    }

    fclose(file);

    // Sort the characters in ASCII order
    qsort(charInfo, 256, sizeof(struct CharInfo), compareChars);

    // Generate and print the desired output
    printf("Total Number of Chars = %d\n", totalChars);
    printf("Total Unique Chars = %d\n", totalUniqueChars);
    printf("\n");

    for (int i = 0; i < 256; i++) {
        if (charInfo[i].frequency > 0) {
            printf("Ascii Value: %d, Char: %c, Count: %d, Initial Position: %d\n",
                (int)charInfo[i].character, charInfo[i].character, charInfo[i].frequency, charInfo[i].initial_position);
        }
    }
}


void w(const char *inputFileName) {
    FILE *file = fopen(inputFileName, "r");
    if(file == NULL) {
        printf("ERROR: Can't open input file\n");
        exit(1);
    }


    // Initialize word information array
    struct WordInfo *wordInfo = NULL;
    int maxWords = 100; // Initial maximum number of words
    int totalWords = 0;
    int totalUniqueWords = 0;

    wordInfo = (struct WordInfo *)malloc(maxWords * sizeof(struct WordInfo));
    if (wordInfo == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return;
    }

    // Read and process words from the file
    char buffer[1024];
    int position = 0;

    while (fscanf(file, "%s", buffer) != EOF) {
        totalWords++;

        // Check if the word is already in the wordInfo array
        int found = 0;
        for (int i = 0; i < totalUniqueWords; i++) {
            if (strcmp(wordInfo[i].word, buffer) == 0) {
                found = 1;
                wordInfo[i].frequency++;
                break;
            }
        }

        if (!found) {
            // Add a new unique word to wordInfo array
            if (totalUniqueWords >= maxWords) {
                maxWords *= 2; // Double the maximum number of words if needed
                wordInfo = (struct WordInfo *)realloc(wordInfo, maxWords * sizeof(struct WordInfo));
                if (wordInfo == NULL) {
                    perror("Memory allocation failed");
                    fclose(file);
                    return;
                }
            }

            wordInfo[totalUniqueWords].word = strdup(buffer);
            wordInfo[totalUniqueWords].frequency = 1;
            wordInfo[totalUniqueWords].initial_position = position;
            totalUniqueWords++;
        }

        position++;
    }

    fclose(file);

    // Sort the words in alphabetical order
    qsort(wordInfo, totalUniqueWords, sizeof(struct WordInfo), compareWords);

    // Generate and print the desired output
    printf("Total Number of Words: %d\n", totalWords);
    printf("Total Unique Words: %d\n", totalUniqueWords);
    printf("\n");

    for (int i = 0; i < totalUniqueWords; i++) {
        printf("Word: %s, Freq: %d, Initial Position: %d\n",
            wordInfo[i].word, wordInfo[i].frequency, wordInfo[i].initial_position);
        free(wordInfo[i].word);
    }
    free(wordInfo);
}

void l(const char *inputFileName) {
    FILE *file = fopen(inputFileName, "r");
    if(file == NULL) {
        printf("ERROR: Can't open input file\n");
        exit(1);
    }
    // Initialize line information array
    struct LineInfo *lineInfo = NULL;
    int maxLines = 100; // Initial maximum number of lines
    int totalLines = 0;
    int totalUniqueLines = 0;

    lineInfo = (struct LineInfo *)malloc(maxLines * sizeof(struct LineInfo));
    if (lineInfo == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return;
    }

    // Read and process lines from the file
    char buffer[1024];
    int position = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        totalLines++;

        // Remove trailing newline character, if any
        size_t length = strlen(buffer);
        if (length > 0 && buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0';
        }

        // Check if the line is already in the lineInfo array
        int found = 0;
        for (int i = 0; i < totalUniqueLines; i++) {
            if (strcmp(lineInfo[i].line, buffer) == 0) {
                found = 1;
                lineInfo[i].frequency++;
                break;
            }
        }

        if (!found) {
            // Add a new unique line to lineInfo array
            if (totalUniqueLines >= maxLines) {
                maxLines *= 2; // Double the maximum number of lines if needed
                lineInfo = (struct LineInfo *)realloc(lineInfo, maxLines * sizeof(struct LineInfo));
                if (lineInfo == NULL) {
                    perror("Memory allocation failed");
                    fclose(file);
                    return;
                }
            }

            lineInfo[totalUniqueLines].line = strdup(buffer);
            lineInfo[totalUniqueLines].frequency = 1;
            lineInfo[totalUniqueLines].initial_position = position;
            totalUniqueLines++;
        }

        position++;
    }

    fclose(file);

    // Sort the lines in alphabetical order
    qsort(lineInfo, totalUniqueLines, sizeof(struct LineInfo), compareLines);

    // Generate and print the desired output
    printf("Total Number of Lines: %d\n", totalLines);
    printf("Total Unique Lines: %d\n", totalUniqueLines);
    printf("\n");

    for (int i = 0; i < totalUniqueLines; i++) {
        printf("Line: %s, Freq: %d, Initial Position: %d\n",
            lineInfo[i].line, lineInfo[i].frequency, lineInfo[i].initial_position);
        free(lineInfo[i].line);
    }
    free(lineInfo);
}

void Lw(const char *inputFileName) {
    FILE *file = fopen(inputFileName, "r");
    if(file == NULL) {
        printf("ERROR: Can't open input file\n");
        exit(1);;
    }


    // Initialize variables to keep track of longest words
    int maxWordLength = 0;
    struct wordInfo longestWords[100];
    int longestWordCount = 0;

    // Read and process words from the file
    char buffer[1024];

    while (fscanf(file, "%s", buffer) != EOF) {
        int wordLength = strlen(buffer);

    if (wordLength > maxWordLength) {
    // Free previously stored longest words
    	for (int i = 0; i < longestWordCount; i++) {
        	free(longestWords[i].word);
    	}
    	longestWordCount = 0; // Reset count before adding new longest word

    // Set new longest word
    	maxWordLength = wordLength;
    	longestWords[0].word = strdup(buffer);
    	longestWords[0].length = wordLength;
    	longestWordCount = 1;
        } else if (wordLength == maxWordLength) {
            // Found a word with the same length as the current longest words
            longestWords[longestWordCount].word = strdup(buffer);
            longestWords[longestWordCount].length = wordLength;
            longestWordCount++;
        }
    }

    fclose(file);

    // Sort the longest words alphabetically
    qsort(longestWords, longestWordCount, sizeof(struct WordInfo), compareWords);
    removeDuplicates(longestWords, &longestWordCount);

    // Print the desired output
    printf("Longest Word is %d characters long:\n", maxWordLength);

    for (int i = 0; i < longestWordCount; i++) {
        printf("\t%s\n", longestWords[i].word);
        free(longestWords[i].word);
    }
}

void Ll(const char *inputFileName) {
    FILE *file = fopen(inputFileName, "r");
    if(file == NULL) {
        printf("ERROR: Can't open input file\n");
        exit(1);
    }


    // Initialize variables to keep track of longest lines
    int maxLineLength = 0;
    struct lineInfo longestLines[100];
    int longestLineCount = 0;

    // Read and process lines from the file
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        int lineLength = strlen(buffer);

        if (lineLength > maxLineLength) {
            // Found a longer line, reset the longestLines array
            maxLineLength = lineLength;
            longestLines[0].line = strdup(buffer);
            longestLines[0].length = lineLength;
            longestLineCount = 1;
        } else if (lineLength == maxLineLength) {
            // Found a line with the same length as the current longest lines
            longestLines[longestLineCount].line = strdup(buffer);
            longestLines[longestLineCount].length = lineLength;
            longestLineCount++;
        }
    }
    maxLineLength--;


    fclose(file);
    
    // Sort the longest lines alphabetically
    qsort(longestLines, longestLineCount, sizeof(struct LineInfo), compareLines);

    // Print the desired output
    printf("Longest Line is %d characters long:\n", maxLineLength);

    for (int i = 0; i < longestLineCount; i++) {
        printf("\t%s", longestLines[i].line);
        free(longestLines[i].line);
    }

}

void processCommand(int argc, char *argv[]) {
    const char *inputFileName = NULL;
    const char *outputFileName = NULL;

    int tab = 0;

    char *selectedFlags[argc]; // Array to store selected flags in order
    int selectedFlagsCount = 0;
    FILE *outputFile = stdout;  // Default to stdout

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                inputFileName = argv[i + 1];
                i++;  // Skip the next argument since it's the input file name
            } else {
                printf( "ERROR: No Input File Provided\n");
                return;
            }
        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                outputFileName = argv[i + 1];
                outputFile = freopen(outputFileName, "w", stdout);
                i++;  // Skip the next argument since it's the output file name
            } else {
                printf("ERROR: No Output File Provided\n");
                return;
            }
        } else {
            // Store selected analysis flags in the order they appear
            selectedFlags[selectedFlagsCount++] = argv[i];
        }
    }

    if (inputFileName == NULL) {
        printf("ERROR: No Input File Provided\n");
        return;
    }

    FILE *input = fopen(inputFileName, "r");
    if (input == NULL) {
        printf("ERROR: Can't open input file\n");
        return;
    }

    if (fgetc(input) == EOF) {
        printf("ERROR: Input File Empty\n");
        fclose(input);
        return;        
        
    } 
    // Call analysis functions in the order they were selected
    for (int i = 0; i < selectedFlagsCount; i++) {
        const char *flag = selectedFlags[i];
        if (strcmp(flag, "-c") == 0) {
            if (tab)
                printf("\n");
            tab = 1;
            c(inputFileName);
        } else if (strcmp(flag, "-w") == 0) {
            if (tab)
                printf("\n");
            w(inputFileName);
        } else if (strcmp(flag, "-l") == 0) {
            if (tab)
                printf("\n");
            l(inputFileName);
            tab = 1;
        } else if (strcmp(flag, "-Lw") == 0) {
            if (tab)
                printf("\n");
            Lw(inputFileName);
            tab = 1;
        } else if (strcmp(flag, "-Ll") == 0) {
            if (tab)
                printf("\n");
            Ll(inputFileName);
            tab = 1;
        } else {
            fprintf(stderr, "ERROR: Invalid Flag Types\n");
            exit(1);
        }
    }

    // Close the output file if it's not stdout
    if (outputFile != stdout) {
        fclose(outputFile);
    }
}
void batchMode(const char *batchFileName) {
    FILE *batchFile = fopen(batchFileName, "r");
    if (batchFile == NULL) {
        printf("ERROR: Can't open batch file\n");
        exit(1);
    }

    // Move to the end of the file
    fseek(batchFile, 0, SEEK_END);
    // Check the position which represents the file's size
    long fileSize = ftell(batchFile);
    if (fileSize == 0) {
        printf("ERROR: Batch File Empty\n");
        fclose(batchFile);
        exit(1);
    }

    // If the file is not empty, rewind to the beginning of the file
    rewind(batchFile);

    char line[1024];  // Assuming no line exceeds 1024 characters
    while (fgets(line, sizeof(line), batchFile) != NULL) {
        // Remove the trailing newline character, if present
        line[strcspn(line, "\n")] = 0;

        // Tokenize the line and execute the command
        char *args[20];  // Assuming no more than 20 arguments per command
        int argc = 0;
        char *token = strtok(line, " ");
        while (token != NULL) {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }

        if (argc > 0) {
            processCommand(argc, args);  // Function to process each command
        }
    }
    fclose(batchFile);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("USAGE:\n\t./MADCounter -f <input file> -o <output file> -c -w -l -Lw -Ll\n\t\tOR\n\t./MADCounter -B <batch file>\n");
        return 1; // Exit with an error code
    }

    char *selectedFlags[argc]; // Array to store selected flags in order
    int selectedFlagsCount = 0;

    const char *inputFileName = NULL;
    const char *outputFileName = NULL;
    int tab = 0;

    FILE *outputFile = stdout; // Default to stdout

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-B") == 0) {
            if (i + 1 < argc) {
                batchMode(argv[i + 1]);
                return 0;  // Exit after processing the batch file
            } else {
                printf("ERROR: No Batch File Provided\n");
                exit(1);
            }
        } else if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                inputFileName = argv[i + 1];
                i++; // Increment i to skip the input file name argument
            } else {
                printf("ERROR: No Input File Provided\n");
                exit(1); // Exit with an error code
            }
        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                outputFileName = argv[i + 1];
                i++; // Increment i to skip the output file name argument
            } else {
                printf("ERROR: No Output File Provided\n");
                exit(1); // Exit with an error code
            }
        } else {
            // Store selected analysis flags in the order they appear
            selectedFlags[selectedFlagsCount++] = argv[i];
        }
    }

    if (inputFileName == NULL) {
        printf("ERROR: No Input File Provided\n");
        return 1; // Exit with an error code
    }
    outputFile = freopen(outputFileName, "w", stdout);

    FILE *input = fopen(inputFileName, "r");
    if (input == NULL) {
        printf("ERROR: Can't open input file\n");
        exit(1);
    }

    // Check if input file is empty
    if (fgetc(input) == EOF) {
        printf("ERROR: Input File Empty\n");
        fclose(input);
        fclose(outputFile); // Close the output file if it's not stdout
        exit(1);
    }

    // Call analysis functions in the order they were selected
    for (int i = 0; i < selectedFlagsCount; i++) {
        const char *flag = selectedFlags[i];
        if (strcmp(flag, "-c") == 0) {
            if (tab)
                printf("\n");
            tab = 1;
            c(inputFileName);
        } else if (strcmp(flag, "-w") == 0) {
            if (tab)
                printf("\n");
            w(inputFileName);
        } else if (strcmp(flag, "-l") == 0) {
            if (tab)
                printf("\n");
            l(inputFileName);
            tab = 1;
        } else if (strcmp(flag, "-Lw") == 0) {
            if (tab)
                printf("\n");
            Lw(inputFileName);
            tab = 1;
        } else if (strcmp(flag, "-Ll") == 0) {
            if (tab)
                printf("\n");
            Ll(inputFileName);
            tab = 1;
        } else {
            printf("ERROR: Invalid Flag Types\n");
            return 1; // Exit with an error code
        }
    }

    // Close the output file if it's not stdout
    if (outputFile != stdout) {
        fclose(outputFile);
    }

    return 0; // Successful execution
}
