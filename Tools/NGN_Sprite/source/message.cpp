/******************************************************************************

    Conversor de PNG a Sprite (.spr) para N'gine
	- Sistema de mensages -

    Proyecto iniciado el 11 de Febrero del 2016
    (cc) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/

******************************************************************************/



/*** Includes ***/

// C++
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

// Includes del proyecto
#include "message.h"
#include "defines.h"



/*** Constructor ***/
Message::Message(std::string pname) {

    // Guarda el nombre del ejecutable del programa
    program_name = pname;

}



/*** Destructor ***/
Message::~Message() {

}



/*** Texto de encabezado de la aplicacion ***/
void Message::AppHeader() {

    // Genera las cadenas de texto adicionales
    std::string ngn_version = std::to_string(NGN_VERSION_MAJOR) + "." + std::to_string(NGN_VERSION_MINOR) + "." + std::to_string(NGN_VERSION_PATCH) + "-" + NGN_VERSION_METADATA;
    std::string program_version = std::to_string(PROGRAM_VERSION_MAJOR) + "." + std::to_string(PROGRAM_VERSION_MINOR) + "." + std::to_string(PROGRAM_VERSION_PATCH) + "-" + PROGRAM_VERSION_METADATA;

    // Imprime el texto
    std::cout <<
    std::endl <<
    std::endl << TEXT_DASHED_LINE <<
    std::endl <<
    std::endl << "  " << PROGRAM_NAME << ". (v" << program_version << ")." <<
    std::endl << "  For N'gine Version " << ngn_version << " or higher." <<
    std::endl << "  (cc) " << CC_YEARS << " by Cesar Rincon." <<
    std::endl << "  https://nightfoxandco.com" <<
    std::endl << "  contact@nightfoxandco.com" <<
    std::endl <<
    std::endl << TEXT_DASHED_LINE <<
    std::endl << std::endl;

}



/*** Texto "Como obtener ayuda" ***/
void Message::QuestionMarkForHelp() {

    std::cout << "Type " << program_name << " " << ARG_HELP << " to display the on-screen help manual." << std::endl << std::endl;

}



/*** Texto "Error de uso" ***/
void Message::UseError() {

    std::cout << "Command line error." << std::endl;

}



/*** Texto "Manual de uso" ***/
void Message::UserManual() {

    std::cout << std::endl;
    std::cout << program_name << " INPUT_FILE.PNG " << ARG_WIDTH << " xxxx " << ARG_HEIGHT << " xxxx [" << ARG_OUT_FILE << " filename] [" << ARG_STRIP << "]" << std::endl;
    std::cout << std::endl;
    std::cout << ARG_HELP << " Displays the on-screen user manual (this screen)." << std::endl;
    std::cout << ARG_WIDTH << " WIDTH of the frame. (From [" << MIN_FRAME_SIZE << "] to [" << MAX_FRAME_SIZE << "] pixels)." << std::endl;
    std::cout << ARG_HEIGHT << " HEIGHT of the frame. (From [" << MIN_FRAME_SIZE << "] to [" << MAX_FRAME_SIZE << "] pixels)." << std::endl;
    std::cout << ARG_OUT_FILE << " Name for the output file, without extension (" << SPR_EXTENSION << " will be added automaticaly)." << std::endl;
    std::cout << ARG_STRIP << " Generates a PNG file with the frames of the sprite in strip format." << std::endl;
    std::cout << std::endl;

}
