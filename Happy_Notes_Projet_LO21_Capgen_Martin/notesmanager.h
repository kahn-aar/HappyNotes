#ifndef NOTESMANAGER_H
#define NOTESMANAGER_H

#include <QTextStream>
#include <QMap>
#include <QSet>
#include "notesfactory.h"
#include "exportstrategy.h"
#include "tags.h"
#include "note.h"

using namespace std;
class NotesFactory;
class ExportStrategy;
class Tags;


/*!
 * \brief La classe NotesManager est la classe centrale de l'application.
 *
 * Il s'agit d'un singleton car il ne peut exister qu'une seule instance de la classe NotesManager.
 * Elle compose toutes les notes ouvertes, et permet de gérer la corbeille et les notes filtrées.
 * Elle permet de charger et de sauvegarder toutes les notes.
 */
class NotesManager {
protected:
    NotesManager();
    ~NotesManager();
    NotesManager(const NotesManager&); // non défini mais privé pour empêcher la duplication
    NotesManager& operator=(const NotesManager&); // même chose
    static NotesManager* instance; /**< Il s'agit du pointeur sur l'unique instance de NotesManager. Fait partie du disign pattern singleton.*/
    QString workspace; /**< Désigne le chemin vers le dossier où sont stockées les notes.*/

    QMap<QString, NotesFactory*> factories; /**< Il s'agit d'une map mettant une chaine de caractère en lien avec une NotesFactory. Il y a une entrée par type de Note*/
    QMap<QString, ExportStrategy*> strategies; /**< Il s'agit d'une map mettant une chaine de caractère en lien avec une ExportStrategy. Il y a une entrée par type d'export*/

    QSet<Note*> notes; /**< Il s'agit de la liste de toutes les notes de l'application.*/
    QSet<Note*> filteredNotes; /**< Il s'agit de la liste filtrée par le filtre de l'application;*/
    QSet<Note*> corbeille; /**< Il s'agit de la liste des notes supprimée, permettant de gérer la restauration ainsi que la suppression totale des notes.*/


public:

    static NotesManager* getInstance();
    static void libererInstance();
    /*!
     * \brief Renvoie le workspace
     * \return
     */
    QString getWorkSpace() const {return workspace;}
    void setWorkspace(const QString& ws);
    void setCopyWorkspace(const QString& ws);
    void setChangeWorkspace(QString &desc);
    void exporterNote(const QString& strat, Note* n);
    Note* getNewNote(const QString& type, const QString& title="");
    Note* getNote(unsigned int i, const QString& type, const QString& title);
    Note* getNote(const QString& titre);
    Note* getNote(unsigned int i);
    /*!
     * \brief renvoye le nombre de notes dans la corbeille
     */
    unsigned int getNbBinNotes() const {return corbeille.size();}
    Note* getBinNote(const QString& titre);
    bool isInBin(Note *note);
    /*!
     * \brief Renvoie le nombre de notes dans la liste de notes.
     * \return
     */
    unsigned int getNbNotes() const {return notes.size();}
    /*!
     * \brief Supprime la Note note de la liste de notes.
     * \param note
     */
    void supprNote(Note* note);
    void saveNotes();
    /*!
     * \brief Vide la liste de Note filtrées.
     */
    void deleteFiltered(){filteredNotes.clear();}
    void initializeFiltered();
    void filter(const QString& filtre);
    void filter_content(const QString& filtre);
    void trash(Note* note);
    void restaure(Note* note, const QString& filtre);
    void restaureAll(const QString& filtre);
    void destroy(Note* note);
    void clearBin();
    void removeTag(Tags* tag);
    const QString getType(const Note& note) const;

    /*!
     * \brief La classe Iterator est un design pattern Iterator
     *
     * Cette classe offre un Itérateur pour la liste de notes du NotesManager. Nous avons utilisé un design pattern Adapteur d'objet sur l'itérateur de QSet afin de l'implémenter.
     */
    class Iterator {
        QSet<Note*>::iterator itSet;
    public:
        Iterator();
        Iterator(QSet<Note*>::iterator iterator) : itSet(iterator){}
        Iterator operator++() {++itSet; return itSet;}
        Iterator operator--() {--itSet; return itSet;}
        Iterator operator++(int) {return itSet++;}
        Iterator operator--(int) {return itSet--;}
        Note* operator*() {return *itSet;}
        bool operator==(const Iterator& it) const {return itSet == it.itSet;}
        bool operator!=(const Iterator& it) const {return itSet != it.itSet;}

    };
    /*!
     * \brief begin
     * \return
     */
    Iterator begin(){return Iterator(notes.begin());}
    /*!
     * \brief end
     * \return
     */
    Iterator end() {return Iterator(notes.end());}


    /*!
     * \brief begin_filt
     * \return
     */
    Iterator begin_filt(){return Iterator(filteredNotes.begin());}
    /*!
     * \brief end_filt
     * \return
     */
    Iterator end_filt() {return Iterator(filteredNotes.end());}


    /*!
     * \brief begin_bin
     * \return
     */
    Iterator begin_bin(){return Iterator(corbeille.begin());}
    /*!
     * \brief end_bin
     * \return
     */
    Iterator end_bin() {return Iterator(corbeille.end());}


    /*!
     * \brief La classe Const_Iterator est un design pattern Iterator
     *
     * Cette classe offre un Itérateur constant pour la liste de notes du NotesManager. Nous avons utilisé un design pattern Adapteur d'objet sur l'itérateur constant de QSet afin de l'implémenter.
     */
    class Const_Iterator
    {
        QSet<Note*>::const_iterator itSet;


    public:
        Const_Iterator();
        Const_Iterator(QSet<Note*>::const_iterator iterator);
        Const_Iterator operator++() {++itSet; return itSet;}
        Const_Iterator operator--() {--itSet; return itSet;}
        Const_Iterator operator++(int) {return itSet++;}
        Const_Iterator operator--(int) {return itSet--;}
        const Note* operator*() {return *itSet;}
        bool operator==(const Const_Iterator& it) const {return itSet == it.itSet;}
        bool operator!=(const Const_Iterator& it) const {return itSet != it.itSet;}

    };

    /*!
     * \brief cbegin
     * \return un itérateur constant sur le début des notes
     */
    Const_Iterator cbegin() const {return Const_Iterator(notes.cbegin());}

    /*!
     * \brief cbegin
     * \return un itérateur constant sur la fin notes
     */
    Const_Iterator cend() const {return Const_Iterator(notes.cend());}
 };

#endif // NOTESMANAGER_H
