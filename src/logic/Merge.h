//
// Created by necr on 6/17/2023.
//

#ifndef MAPPINGLOGIC_MERGE_H
#define MAPPINGLOGIC_MERGE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

#include <windows.h>
#include <ShlObj.h>
#include <commdlg.h>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/error/en.h>

using namespace std;

class Merge {

public:
    /*
     * Process the merge logic.
     * This method will prompt the user for the names of the files to merge and the name of the output file.
     *
     * @throws runtime_error if any of the files cannot be opened or parsed
     */
    static void processMerge();

private:
    /*
    * Merge the contents of the given JSON files into a single JSON file.
    * The output file will be formatted using pretty printing.
    *
    * @param filenames - the names of the files to merge
    * @param outputFilename - the name of the file to write the merged contents to
    *
    * @throws runtime_error if any of the files cannot be opened or parsed
    */
    static void mergeMappings(const vector<string>& filenames, const char* outputFilename);

    /*
     * Open a file selection dialog and return the selected file paths.
     *
     * @return a vector containing the selected file paths
     */
    static vector<string> openMultipleFileSelectionDialog() {
        OPENFILENAME ofn;
        TCHAR szFile[260 * MAX_PATH] = { 0 };  // Buffer to store the selected file paths

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = nullptr;
        ofn.lpstrFilter = "JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";  // Filter for JSON files and all file types
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = MAX_PATH * 100; // Increased buffer size for multiple files
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER; // Added flag for multiple file selection

        vector<string> selectedFiles;

        // Open the file dialog
        if (GetOpenFileName(&ofn))
        {
            TCHAR* p = szFile;
            string directory(p);
            p += directory.size() + 1;

            while (*p)
            {
                string filename(p);
                string fullPath = directory + "\\" + filename;
                selectedFiles.push_back(fullPath);

                p += filename.size() + 1;
            }
        } else {
            DWORD error = CommDlgExtendedError();
            cout << "Error occurred while opening file selection dialog. Error code: " << error << endl;
        }

        cout << "Selected files: " << endl;
        for (const auto& file : selectedFiles) {
            cout << file << endl;
        }

        return selectedFiles;
    }

    /*
     * Open a file save dialog and return the selected file path.
     * The default file name is "merged_mappings.json".
     *
     * @return the selected file path
     */
    static string openFileSaveDialog() {
        string filePath;

        OPENFILENAME ofn;
        char fileName[MAX_PATH] = "merged_mappings.json";  // Default file name

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = nullptr;
        ofn.hInstance = GetModuleHandle(nullptr);  // Set the handle to the current instance
        ofn.lpstrFilter = "JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";  // Filter for JSON files and all file types
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrTitle = "Save Merged Mappings";    // Set the title of the dialog window
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_EXPLORER;

        if (GetSaveFileName(&ofn) == TRUE) {
            filePath = fileName;
        }

        return filePath;
    }

    /*
     * Open a file save dialog and return the selected file path. Low-level solution.
     * The default file name is "merged_mappings.json".
     *
     * @return the selected file path
     */
    static const char* openFileSaveDialogLowLevel() {
        static char filePath[MAX_PATH];

        OPENFILENAME ofn;
        char fileName[MAX_PATH] = "merged_mappings.json";  // Default file name

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = nullptr;
        ofn.hInstance = GetModuleHandle(nullptr);  // Set the handle to the current instance
        ofn.lpstrFilter = "JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";  // Filter for JSON files and all file types
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrTitle = "Save Merged Mappings";    // Set the title of the dialog window
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_EXPLORER;

        if (GetSaveFileName(&ofn) == TRUE) {
            strncpy(filePath, fileName, MAX_PATH);
        } else {
            filePath[0] = '\0';  // Set an empty string if no file is selected
        }

        return filePath;
    }


    /*
     * Open a folder selection dialog and return the selected folder path.
     *
     * @return the selected folder path
     */
    [[maybe_unused]]
    static string openFolderSelectionDialog() {
        string folderPath;

        BROWSEINFO bi;
        ZeroMemory(&bi, sizeof(bi));
        bi.hwndOwner = nullptr;
        bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
        bi.lpszTitle = "Select Folder";    // Set the title of the dialog window
        bi.lpfn = nullptr;  // No callback function
        bi.lParam = 0;      // No custom data
        LPITEMIDLIST pIDL = SHBrowseForFolder(&bi);

        if (pIDL != nullptr) {
            char folderPathBuffer[MAX_PATH];
            if (SHGetPathFromIDList(pIDL, folderPathBuffer)) {
                folderPath = folderPathBuffer;
            }
            CoTaskMemFree(pIDL);
        }

        return folderPath;
    }

};


#endif //MAPPINGLOGIC_MERGE_H
