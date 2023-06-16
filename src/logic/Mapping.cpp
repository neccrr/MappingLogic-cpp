//
// Created by necr on 6/16/2023.
//
// Original: https://github.com/neccrr/MappingLogic/blob/main/Mapping.py

#include "Mapping.h"

// if i didn't put this the build will fail idk why
vector<map<string, string>> Mapping::classMappingVector;
vector<map<string, string>> Mapping::fieldMappingVector;
vector<map<string, string>> Mapping::methodMappingVector;

// Processes the class mapping and put it to the classMappingVector
void Mapping::classMapping(const char* filePath, bool saveToJsonFile) {

    // lower-level file I/O functions
    FILE* inputFile = fopen(filePath, "r");
    if (!inputFile) {
        cout << "Error opening file " << filePath << endl;
        return;
    }

    const int bufferSize = 1024; // Adjust buffer size as needed
    char buffer[1024];

    while (fgets(buffer, bufferSize, inputFile) != nullptr) {
        // if line not starts with...
        if (strncmp(buffer, "CL:", strlen("CL:")) != 0) {
            continue;
        }

        // Splits the line by spaces into multiple strings then store them to a vector
        vector<string> classMappingElements = splitString(buffer, ' ');

        // Obfuscated class name | ex: ave (net.minecraft.client.Minecraft)
        string obfuscatedClassName = classMappingElements[1];
        // Mapped (original) class name | ex: net.minecraft.client.Minecraft (ave)
        string mappedClassName = classMappingElements[2];

        // Print the current process state
        cout << "[CLASS] Mapping class " << obfuscatedClassName << " to " << mappedClassName << endl;

        // Create a map object
        map<string, string> classMapping;
        // Store the obfuscated class name and mapped class name to the map object
        classMapping["obfuscatedClassName"] = obfuscatedClassName;
        classMapping["mappedClassName"] = mappedClassName;

        // Put the class mapping to classMappingVector
        classMappingVector.push_back(classMapping);

        // Free the memory
        classMapping.clear();
    }

    fclose(inputFile);

    if (saveToJsonFile) {
        generateJsonFile(classMappingVector, "mappings/processed/class_mapping.json");
    }

}

// Processes the field mapping and put it to the fieldMappingVector
void Mapping::fieldMapping(const char* filePath, bool saveToJsonFile) {

    // lower-level file I/O functions
    FILE* inputFile = fopen(filePath, "r");
    if (!inputFile) {
        cout << "Error opening file " << filePath << endl;
        return;
    }

    const int bufferSize = 1024; // Adjust buffer size as needed
    char buffer[1024];

    while (fgets(buffer, bufferSize, inputFile) != nullptr) {
        // if line not starts with...
        if (strncmp(buffer, "FD:", strlen("FD:")) != 0) {
            continue;
        }

        // Splits the line by spaces into multiple strings then store them to temp vector
        vector<string> fieldMappingElements = splitString(buffer, ' ');

        // Obfuscated field name | ex: h (thePlayer)
        string obfuscatedFieldName = splitString(fieldMappingElements[1], '/')[1];
        // Obfuscated field owner name | ex: ave (net.minecraft.client.Minecraft)
        string obfuscatedFieldOwnerName = splitString(fieldMappingElements[1], '/')[0];
        // Obfuscated full path name | ex: ave/h (net.minecraft.client.Minecraft.thePlayer)
        string obfuscatedFieldFullPathName = fieldMappingElements[1];
        // Obfuscated object type | ex: Lwn; (Lnet/minecraft/entity/player/EntityPlayer;)
        string obfuscatedFieldObjectType = fieldMappingElements[2];

        // Mapped (original) field name | ex: thePlayer (h)
        string mappedFieldName = splitString(fieldMappingElements[3], '/').back();
        // Mapped (original) field owner name | ex: net.minecraft.client.Minecraft (ave)
        auto temp = splitString(fieldMappingElements[3], '/');
        string mappedFieldOwnerName = accumulate(temp.begin(), temp.end() - 1, string(""));

        // Mapped (original) full path name | ex: net.minecraft.client.Minecraft.thePlayer (ave/h)
        string mappedFieldFullPathName = fieldMappingElements[3];
        // Mapped (original) object type ex: Lnet/minecraft/entity/player/EntityPlayer; (Lwn;)
        string mappedFieldObjectType = fieldMappingElements[4];

        // Print the current process state
        cout << "[FIELD] Mapping field " << obfuscatedFieldFullPathName << " to " << mappedFieldFullPathName << endl;

        // Create temp map object
        map<string, string> fieldMapping;
        // Store the field data to the map object
        fieldMapping["obfuscatedFieldName"] = obfuscatedFieldName;
        fieldMapping["obfuscatedFieldOwnerName"] = obfuscatedFieldOwnerName;
        fieldMapping["obfuscatedFieldFullPathName"] = obfuscatedFieldFullPathName;
        fieldMapping["obfuscatedFieldObjectType"] = obfuscatedFieldObjectType;

        fieldMapping["mappedFieldName"] = mappedFieldName;
        fieldMapping["mappedFieldOwnerName"] = mappedFieldOwnerName;
        fieldMapping["mappedFieldFullPathName"] = mappedFieldFullPathName;
        fieldMapping["mappedFieldObjectType"] = mappedFieldObjectType;

        // Put the field mapping to fieldMappingVector
        fieldMappingVector.push_back(fieldMapping);

        // Free the memory
        fieldMapping.clear();
    }

    fclose(inputFile);

    if (saveToJsonFile) {
        generateJsonFile(fieldMappingVector, "mappings/processed/field_mapping.json");
    }
}

// Processes the method mapping and put it to the methodMappingVector
void Mapping::methodMapping(const char* filePath, bool saveToJsonFile) {

    // lower-level file I/O functions
    FILE *inputFile = fopen(filePath, "r");
    if (!inputFile) {
        cout << "Error opening file " << filePath << endl;
        return;
    }

    const int bufferSize = 1024; // Adjust buffer size as needed
    char buffer[1024];

    while (fgets(buffer, bufferSize, inputFile) != nullptr) {
        // if line not starts with...
        if (strncmp(buffer, "MD:", strlen("MD:")) != 0) {
            continue;
        }

        // Splits the line by spaces into multiple strings then store them to temp vector
        vector<string> methodMappingElements = splitString(buffer, ' ');

        // Obfuscated method name | ex: a (init)
        string obfuscatedMethodName = splitString(methodMappingElements[1], '/')[1];
        // Obfuscated method owner name | ex: ave (net.minecraft.client.Minecraft)
        string obfuscatedMethodOwnerName = splitString(methodMappingElements[1], '/')[0];
        // Obfuscated full path name | ex: ave/a (net.minecraft.client.Minecraft.init)
        string obfuscatedMethodFullPathName = methodMappingElements[1];
        // Obfuscated object type | ex: (Lnet/minecraft/client/Minecraft;Lnet/minecraft/util/Session;)V
        string obfuscatedMethodObjectType = methodMappingElements[2];

        // Mapped (original) method name | ex: init (a)
        string mappedMethodName = splitString(methodMappingElements[3], '/').back();
        // Mapped (original) method owner name | ex: net.minecraft.client.Minecraft (ave)
        auto temp = splitString(methodMappingElements[3], '/');
        string mappedMethodOwnerName = accumulate(temp.begin(), temp.end() - 1, string(""));
        // Mapped (original) full path name | ex: net.minecraft.client.Minecraft.init (ave/a)
        string mappedMethodFullPathName = methodMappingElements[3];
        // Mapped (original) object type | ex: (Lnet/minecraft/client/Minecraft;Lnet/minecraft/util/Session;)V
        string mappedMethodObjectType = methodMappingElements[4];

        // Print the current process state
        cout << "[METHOD] Mapping method " << obfuscatedMethodFullPathName << " to " << mappedMethodFullPathName << endl;

        // Create temp map object
        map<string, string> methodMapping;
        // Store the method data to the map object
        methodMapping["obfuscatedMethodName"] = obfuscatedMethodName;
        methodMapping["obfuscatedMethodOwnerName"] = obfuscatedMethodOwnerName;
        methodMapping["obfuscatedMethodFullPathName"] = obfuscatedMethodFullPathName;
        methodMapping["obfuscatedMethodObjectType"] = obfuscatedMethodObjectType;

        methodMapping["mappedMethodName"] = mappedMethodName;
        methodMapping["mappedMethodOwnerName"] = mappedMethodOwnerName;
        methodMapping["mappedMethodFullPathName"] = mappedMethodFullPathName;
        methodMapping["mappedMethodObjectType"] = mappedMethodObjectType;

        // Put the method mapping to methodMappingVector
        methodMappingVector.push_back(methodMapping);

        // Free the memory
        methodMapping.clear();
    }

    fclose(inputFile);

    if (saveToJsonFile) {
        generateJsonFile(methodMappingVector, "mappings/processed/method_mapping.json");
    }
}

// Processes the mapping
void Mapping::processMapping() {
    cout << "Please select an mapping file" << endl;
    char* selectedFile = openFileSelectionDialogLowLevel();
    if (selectedFile == nullptr || selectedFile == string("")) {
        cout << "No file selected" << endl;
        return;
    }
    cout << "Selected File: " << selectedFile << endl;

    auto startTime = chrono::high_resolution_clock::now();

    classMapping(selectedFile, true);

    fieldMapping(selectedFile, true);

    methodMapping(selectedFile, true);

    auto endTime = chrono::high_resolution_clock::now();
    auto elapsedTime = chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    cout << "Elapsed time: " << elapsedTime / 1000.0 << " seconds (" << elapsedTime << " ms)" << endl;
}
