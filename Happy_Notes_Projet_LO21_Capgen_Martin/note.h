#ifndef NOTE_H
#define NOTE_H

#include <QList>
#include <QString>
#include "notesfactory.h"
#include "exportstrategy.h"
#include "notesmanager.h"
#include "tags.h"
#include <QMap>
#include <fstream>

class ExportStrategy;
class Tags;

/*!
 * \brief La classe NotesException Implémente les exceptions dans l'application.
 *
 * Les exceptions permettent de cibler quel est le problème et les gérer de façon efficace.
 */
class NotesException{
public:
    NotesException(const QString& message):info(message){}
    QString getInfo() const { return info; }
private:
    QString info;
};


/*!
 * \brief La classe Note est la classe de base de gestion des notes.
 * Il s'agit d'une classe abstraite permettant d'hériter les arguments aux différents types de notes.
 *
 */
class Note
{

    friend class NotesManager;
    unsigned int ID; /**< ID de la note, pour l'enregistrement*/

protected:
    QString title; /**< Titre de la note*/
    bool isLoaded; /**< Permet de savoir si la note est chargée*/
    bool isModified; /**< Permet de savoir si la note à été modifiée*/
    bool isOnScreen; /**< Permet de savoir si elle est à l'écran*/
    QString exportAsPart(ExportStrategy* es, unsigned int levelTitle);
    QList<Tags*> tags;/**< Il s'agit de la liste des Tags liés à la note*/
public:
    Note(unsigned int id, const QString& t) : ID(id), title(t), isLoaded(false), isModified(false), isOnScreen(false), tags(QList<Tags*>()) {}
    unsigned int getID() const {return ID;}
    const QString& getTitle() const { return title; }
    bool getModified() const {return isModified;}
    bool getLoaded() const {return isLoaded;}
    bool getScreen() const {return isOnScreen;}
    void setLoaded() {isLoaded = true;}
    void setModified(){isModified = true;}
    void setNoModified(){isModified = false;}
    void setOnScreen(){isOnScreen = true;}
    void removeFromScreen(){isOnScreen = false;}
    void setTitle(const QString& t) {title = t;}
    void addTag(Tags *tag);
    void remTag(Tags* tag);
    unsigned int getSize() const {return tags.size();}
    bool isTagged(const QString& name);
    bool isApproxTagged(const QString& name);
    bool release();
    virtual bool load() = 0;
    QString Export(ExportStrategy*);

    /*!
     * \brief La classe TagIterator est un design pattern Iterator
     *
     * Cette classe offre à la classe Note un itérateur pour sa liste de Tags. Nous avons utilisé un design pattern Adapteur d'objet sur l'itérateur de QList afin de l'implémenter.
     */
    class TagIterator
    {
        QList<Tags*>::iterator itList;
    public:
        TagIterator();
        TagIterator(QList<Tags*>::iterator iterator) : itList(iterator){}
        TagIterator operator++() {++itList; return itList;}
        TagIterator operator--() {--itList; return itList;}
        TagIterator operator++(int) {return itList++;}
        TagIterator operator--(int) {return itList--;}
        Tags* operator*() {return *itList;}
        bool operator==(const TagIterator& it) const {return itList == it.itList;}
        bool operator!=(const TagIterator& it) const {return itList != it.itList;}
    };
    TagIterator begin(){return TagIterator(tags.begin());}
    TagIterator end() {return TagIterator(tags.end());}

    /*!
     * \brief La classe ConstTagIterator est un design pattern Iterator constant
     *
     * Cette classe offre à la classe Note un itérateur pour sa liste de Tags. Nous avons utilisé un design pattern Adapteur d'objet sur l'itérateur de QList afin de l'implémenter.
     */
    class ConstTagIterator
    {
        QList<Tags*>::const_iterator itList;
    public:
        ConstTagIterator();
        ConstTagIterator(QList<Tags*>::const_iterator iterator) : itList(iterator){}
        ConstTagIterator operator++() {++itList; return itList;}
        ConstTagIterator operator--() {--itList; return itList;}
        ConstTagIterator operator++(int) {return itList++;}
        ConstTagIterator operator--(int) {return itList--;}
        const Tags* operator*()const {return *itList;}
        bool operator==(const ConstTagIterator& it) const {return itList == it.itList;}
        bool operator!=(const ConstTagIterator& it) const {return itList != it.itList;}
    };
    ConstTagIterator cbegin() const {return ConstTagIterator(tags.begin());}
    ConstTagIterator cend() const {return ConstTagIterator(tags.end());}
};

/*!
 * \brief La classe Article permet de gérer des notes avec un texte.
 */
class Article : public Note{
    friend class NotesManager;

    QString text; /**< Le texte de l'article*/

public:
    Article(unsigned int id, const QString& ti, const QString& te="");
    Article(const Note&);
    const QString& getText() const { return text; }
    void setText(const QString& t);
    bool load();
};

/*!
 * \brief La classe Document permet de gérer des collections de notes.
 * Un Document gère une collection de note
 */
class Document : public Note {
    friend class NotesManager;
    QList<Note*> notes; /**< la liste de notes gérée par le document*/

public:
    Document(unsigned int id, const QString& t);
    Document(const Document& doc);
    Document(const Note& note) : Note(note.getID(), note.getTitle()){}
    //~Document() { delete[] notes; }
    void addNote(Note &note);
    Document&  operator<<(Note& note);
    unsigned int getNb() const {return notes.size();}
    Note* getNote(unsigned int i);
    void removeNote(unsigned int i);
    void removeNote(Note* note);
    void moveArticleUp(unsigned int i);
    void moveArticleDown(unsigned int i);
    bool load();


    /*!
     * \brief La classe Iterator est un design pattern Iterator
     *
     * Cette classe offre la possibilité d'avoir un itérateur sur la collection de notes du Document. Nous avons utilisé un design pattern Adapteur d'objet sur l'itérateur de QList afin de l'implémenter.
     */
    class Iterator
    {
        QList<Note*>::iterator itSet;


    public:
        Iterator();
        Iterator(QList<Note*>::iterator iterator) : itSet(iterator){}
        Iterator operator++() {++itSet; return itSet;}
        Iterator operator--() {--itSet; return itSet;}
        Iterator operator++(int) {return itSet++;}
        Iterator operator--(int) {return itSet--;}
        Note* operator*() {return *itSet;}
        bool operator==(const Iterator& it) const {return itSet == it.itSet;}
        bool operator!=(const Iterator& it) const {return itSet != it.itSet;}
    };
    Iterator begin(){return Iterator(notes.begin());}
    Iterator end() {return Iterator(notes.end());}


    /*!
     * \brief La classe Const_Iterator est un design pattern Iterator constant
     *
     * Cette classe offre la possibilité d'avoir un itérateur constant sur la collection de notes du Document. Nous avons utilisé un design pattern Adapteur d'objet sur l'itérateur constant de QList afin de l'implémenter.
     */
    class Const_Iterator
    {
        QList<Note*>::const_iterator itSet;


    public:
        Const_Iterator();
        Const_Iterator(QList<Note*>::const_iterator iterator) : itSet(iterator){}
        Const_Iterator operator++() {++itSet; return itSet;}
        Const_Iterator operator--() {--itSet; return itSet;}
        Const_Iterator operator++(int) {return itSet++;}
        Const_Iterator operator--(int) {return itSet--;}
        const Note* operator*() const {return *itSet;}
        bool operator==(const Const_Iterator& it) const {return itSet == it.itSet;}
        bool operator!=(const Const_Iterator& it) const {return itSet != it.itSet;}
    };
    Const_Iterator cbegin() const{return Const_Iterator(notes.cbegin());}
    Const_Iterator cend() const{return Const_Iterator(notes.cend());}
};


/*!
 * \brief La classe Media permet la généralisation des différents fichiers binaires qui seront gérés par l'application.
 * Cette classe est abstraite.
 */
class Media : public Note
{
protected:
    QString description; /**< Il s'agit de la description du média*/
    QString path; /**< Il s'agit du chemin vers le fichier binaire*/

public:

    Media(unsigned int id, const QString& t) : Note(id, t){}
    Media(const Note& note) : Note(note.getID(), note.getTitle()) {}
    QString getDesc() const {return description;}
    QString getPath() const {return path;}
    void setDesc(const QString& desc) {description = desc;}
    void setPath(const QString& path_n) {path = path_n;}
    virtual bool load()=0;
};

/*!
 * \brief La classe Image permet de gérer les images.
 *
 * Il s'agit d'une classe dérivée de Média.
 */
class Image : public Media
{

public:
    Image(unsigned int id, const QString& t) : Media(id, t){}
    Image(const Note& note) : Media(note.getID(), note.getTitle()) {}
    bool load();
};

/*!
 * \brief La classe Audio permet de gérer les fichiers audio.
 *
 * Il s'agit d'une classe dérivée de Média.
 */
class Audio : public Media
{

public:
    bool load();
    Audio(unsigned int id, const QString& t) : Media(id, t){}
    Audio(const Note& note) : Media(note.getID(), note.getTitle()) {}
};

/*!
 * \brief La classe Video permet de gérer les fichiers video.
 *
 * Il s'agit d'une classe dérivée de Média.
 */
class Video : public Media
{

public:
    bool load();
    Video(unsigned int id, const QString& t) : Media(id, t){}
    Video(const Note& note) : Media(note.getID(), note.getTitle()) {}
};


#endif // NOTE_H
