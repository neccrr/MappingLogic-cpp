//
// Created by necr on 6/17/2023.
//

#include "Merge.h"

// Merge the contents of the given JSON files into a single JSON file.
void Merge::mergeMappings(const vector<string>& filenames, const char* outputFilename) {
    auto startTime = chrono::high_resolution_clock::now();

    cout << "Merge started..." << endl;

    rapidjson::Document mergeData;

    mergeData.SetArray();

    for (const auto& mapping : filenames) {
        // Open the JSON file
        FILE* file = fopen(mapping.c_str(), "r");
        if (!file) {
            cerr << "Failed to open the file: " << mapping << endl;
            return;
        }

        cout << "Merging file: " << mapping << endl;

        // Create a file read stream for RapidJSON
        char buffer[65536];
        rapidjson::FileReadStream fileStream(file, buffer, sizeof(buffer));

        // Parse the JSON content from the file
        rapidjson::Document document;
        document.ParseStream(fileStream);

        // Close the file
        fclose(file);

        // Check if parsing succeeded
        if (document.HasParseError()) {
            cerr << "Failed to parse JSON: " << rapidjson::GetParseError_En(document.GetParseError()) << endl;
            return;
        }

        cout << "Parsed JSON file: " << mapping << endl;

        // Append the content to the mergeData array
        mergeData.PushBack(document, mergeData.GetAllocator());

        cout << "Appended JSON file: " << mapping << endl;
    }

    // Write JSON to file
    ofstream outputFile(outputFilename);
    if (outputFile.is_open()) {
        rapidjson::StringBuffer buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        mergeData.Accept(writer);
        outputFile << buffer.GetString() << endl;
        outputFile.close();
        cout << "JSON file saved as '" << outputFilename << "'" << endl;
    } else {
        cout << "Error opening output file" << endl;
    }

    auto endTime = chrono::high_resolution_clock::now();
    auto elapsedTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();

    cout << "Elapsed time: " << elapsedTime / 1000.0 << " seconds (" << elapsedTime << " ms)" << endl;
}

void Merge::processMerge() {
    // Prompt user for input filenames using windows file dialog
    cout << "Please select the files to merge" << endl;
    vector<string> filenames = openMultipleFileSelectionDialog();
    // Check if the user selected any files
    if (filenames.empty()) {
        cout << "No files selected" << endl;
        return;
    }

    // Prompt user for output filename using windows save file dialog
    cout << "Please select the output file" << endl;
    const char* outputFilename = openFileSaveDialogLowLevel();
    // Check if the user selected a file
    if (outputFilename == nullptr) {
        cout << "No output file selected" << endl;
        return;
    }

    // Merge the contents of the input files into a single JSON file
    mergeMappings(filenames, outputFilename);
}