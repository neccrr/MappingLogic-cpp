//
// Created by necr on 6/16/2023.
//

#ifndef MAPPINGLOGIC_MAPPING_H
#define MAPPINGLOGIC_MAPPING_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <map>
#include <numeric>
#include <cstdio>
#include <chrono>

#include <windows.h>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>

using namespace std;

class Mapping {

public:
    /*
     * Processes the mapping file
     */
    static void processMapping();

private:
    /*
     * Processes the class mapping
     *
     * @param filePath - path to the class mapping file
     * @param saveToJsonFile - whether to save the processed mapping to a JSON file
     */
    static void classMapping(const char* filePath, bool saveToJsonFile);

    /*
     * Processes the field mapping
     *
     * @param filePath - path to the field mapping file
     * @param saveToJsonFile - whether to save the processed mapping to a JSON file
     */
    static void fieldMapping(const char* filePath, bool saveToJsonFile);

    /*
     * Processes the method mapping
     *
     * @param filePath - path to the method mapping file
     * @param saveToJsonFile - whether to save the processed mapping to a JSON file
     */
    static void methodMapping(const char* filePath, bool saveToJsonFile);

    // Class Mapping vector object
    static vector<map<string, string>> classMappingVector;
    // Field Mapping vector object
    static vector<map<string, string>> fieldMappingVector;
    // Method Mapping vector object
    static vector<map<string, string>> methodMappingVector;

    /*
     * Opens a file selection dialog and returns the selected file path
     *
     * @return string - selected file path
     */
    [[deprecated("Use openFileSelectionDialogLowLevel() instead")]]
    static string openFileSelectionDialog() {
        string filePath;

        OPENFILENAME ofn;
        char fileName[MAX_PATH] = "";

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = nullptr;
        ofn.lpstrFilter = "All Files (*.*)\0*.*\0";  // Filter for all file types
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn) == TRUE) {
            filePath = fileName;
        }

        return filePath;
    }

    /*
     * Opens a file selection dialog and returns the selected file path (lower level I/O solution)
     *
     * @return char* - selected file path (or nullptr if no file is selected)
     */
    static char* openFileSelectionDialogLowLevel() {
        char* filePath = nullptr;

        OPENFILENAME ofn;
        char fileName[MAX_PATH] = "";

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = nullptr;
        ofn.lpstrFilter = "All Files (*.*)\0*.*\0";  // Filter for all file types
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn) == TRUE) {
            size_t length = strlen(fileName) + 1;
            filePath = new char[length];
            strncpy(filePath, fileName, length);
        }

        return filePath;
    }

    /*
     * Splits a string by a delimiter and returns a vector of the substrings
     *
     * @param str - string to split
     * @param delimiter - delimiter to split the string by
     * @return vector<string> - vector of substrings
     */
    static vector<string> splitString(const string& str, char delimiter) {
        vector<string> substrings;
        string::size_type start = 0;
        string::size_type end = str.find(delimiter);

        while (end != string::npos) {
            substrings.push_back(str.substr(start, end - start));
            start = end + 1;
            end = str.find(delimiter, start);
        }

        substrings.push_back(str.substr(start));

        return substrings;
    }

    /*
     * Splits a string by a delimiter and returns a vector of the substrings (lower level I/O solution)
     *
     * @param buffer - string buffer to split
     * @param delimiter - delimiter to split the string by
     * @return vector<string> - vector of substrings
     */
    static vector<string> splitString(const char* buffer, char delimiter) {
        vector<string> substrings;
        const char* start = buffer;
        const char* end = strchr(buffer, delimiter);

        while (end != nullptr) {
            substrings.emplace_back(start, end - start);
            start = end + 1;
            end = strchr(start, delimiter);
        }

        substrings.emplace_back(start);

        return substrings;
    }

    /*
     * Generates a JSON file from a vector of mappings
     *
     * @param mappings - vector of mappings
     * @param filePath - path to the output JSON file
     */
    static void generateJsonFile(const vector<map<string, string>>& mappings, const string& filePath) {
        auto startTime = chrono::high_resolution_clock::now();

        // Convert mappings to JSON
        rapidjson::Document document;
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        
        document.SetArray();

        for (const auto& mapping : mappings) {
            rapidjson::Value mappingObject(rapidjson::kObjectType);
            for (const auto& entry : mapping) {
                rapidjson::Value key(entry.first.c_str(), allocator);
                rapidjson::Value value(entry.second.c_str(), allocator);

                // Remove newline characters from the value
                string sanitizedValue = entry.second;
                size_t pos = sanitizedValue.find('\n');
                if (pos != string::npos) {
                    sanitizedValue = sanitizedValue.substr(0, pos);
                }

                value.SetString(sanitizedValue.c_str(), allocator);

                mappingObject.AddMember(key, value, allocator);

            }
            document.PushBack(mappingObject, allocator);
        }

        // Write JSON to file
        ofstream outputFile(filePath);
        if (outputFile.is_open()) {
            rapidjson::StringBuffer buffer;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
            document.Accept(writer);
            outputFile << buffer.GetString() << endl;
            outputFile.close();
            cout << "JSON file saved as '" << filePath << "'" << endl;
        } else {
            cout << "Error opening output file" << endl;
        }

        auto endTime = chrono::high_resolution_clock::now();
        auto elapsedTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
        cout << "generateJsonFile() took " << elapsedTime / 1000.0 << " seconds (" << elapsedTime << " ms)" << endl;
    }

};


#endif //MAPPINGLOGIC_MAPPING_H
