/*
 * File: gfilechooser.cpp
 * ----------------------
 * This file implements the operations declared in the gfilechooser.h file.
 * See gfilechooser.h for documentation of each member.
 * 
 * @author Marty Stepp
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * @since 2014/07/09
 */

#include "gfilechooser.h"
#include "private/platform.h"

std::string GFileChooser::showOpenDialog(const std::string& currentDir, const std::string& fileFilter) {
    return stanfordcpplib::getPlatform()->gfilechooser_showOpenDialog(currentDir, fileFilter);
}

std::string GFileChooser::showSaveDialog(const std::string& currentDir, const std::string& fileFilter) {
    return stanfordcpplib::getPlatform()->gfilechooser_showSaveDialog(currentDir, fileFilter);
}
