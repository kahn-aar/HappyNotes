#ifndef NOTESFACTORY_H
#define NOTESFACTORY_H
#include "note.h"
#include <QMap>
#include <ctime>
#include <iostream>

using namespace std;

class Note;
class Article;
class Document;
class Audio;
class Video;
class Image;

/*!
 * \brief La classe NotesFactory est la superclasse des factories.
 *
 * Cette classe implémente la méthode newID() qui génère les ID des notes. De plus, cette classe donne la liste des factories au NotesManager.
 */
class NotesFactory
{
protected:
    /*!
     * \brief Génère un ID
     * \return
     */
    unsigned int newId() const;

public:
    virtual Note* buildNote(unsigned int id, QString title)const=0;
    virtual Note* buildNewNote(QString title) const=0;
    virtual Note* buildNoteCopy(const Note* note) const=0;
    /*!
     * \brief Liste des factories
     * \return
     * Renvoie une MAP liant une QString (le nom du type de la Note) et la factory correspondante
     */
    static QMap<QString, NotesFactory*> getFactories();
};

/*!
 * \brief La classe ImageFactory permet de créer des images.
 */
class ImageFactory : public NotesFactory
{

public:
    Note* buildNote(unsigned int id, QString title)const;
    Note* buildNewNote(QString title)const;
    Note* buildNoteCopy(const Note* note)const;
};

/*!
 * \brief La classe DocumentFactory permet de créer des Document.
 */
class DocumentFactory : public NotesFactory
{

public:
    Note* buildNote(unsigned int id, QString title) const;
    Note* buildNewNote(QString title) const;
    Note* buildNoteCopy(const Note* note) const;

};
/*!
 * \brief La classe ArticleFactory permet de créer des Article.
 */
class ArticleFactory : public NotesFactory
{
public:
   Note* buildNote(unsigned int id, QString title) const;
   Note* buildNewNote(QString title) const;
   Note* buildNoteCopy(const Note* note) const ;
};

/*!
 * \brief La classe AudioFactory permet de créer des Audio.
 */
class AudioFactory : public NotesFactory
{
public:
    Note* buildNote(unsigned int id, QString title) const;
    Note* buildNewNote(QString title) const;
    Note* buildNoteCopy(const Note* note) const;
};

/*!
 * \brief La classe VideoFactory permet de créer des Video.
 */
class VideoFactory : public NotesFactory
{
public:
    Note* buildNote(unsigned int id, QString title) const;
    Note* buildNewNote(QString title) const;
    Note* buildNoteCopy(const Note* note) const;
};

#endif // NOTESFACTORY_H
