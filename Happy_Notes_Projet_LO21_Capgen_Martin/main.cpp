#include <QApplication>
#include <QString>
#include <iostream>
#include "notesmanager.h"
#include "notesediteur.h"

/* Documentation tag for Doxygen
 */

/*! \mainpage Documentation de Happy Notes
 *
 * \section intro_sec Introduction
 *
 * Le logiciel Happy Notes a été conçu et programmé par Capgen Léa et Martin Nicolas dans le cadre de l'UV LO21.
 *
 * \section install_sec Installation
 *
 * \subsection tools_subsec Framework necessaire:
 * - QT version 5.02
 *
 * \section copyright Copyright and License
 * Ce logiciel est open source.
 *
 * <BR><BR>
 *
 */

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    NotesEditeur* editeur = NotesEditeur::getInstance();
    editeur->show();
    return app.exec();
}
