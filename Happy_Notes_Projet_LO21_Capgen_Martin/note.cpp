#include "note.h"
#include <QFile>
#include <QTextStream>
#include <QList>
/*
QString Note::Export(ExportStrategy* es)
{
    es->header(this);
    this->exportAsPart(es, 0);
    es->footer(this);
}
*/

/*!
 * \brief Note::addTag
 * \param tag
 * Cette fonction ajoute le Tag "tag" à la liste des tags de la note
 */
void Note::addTag(Tags* tag)
{
    bool exist = false;
    if(tags.empty()==false)
    {
        for(Note::TagIterator it = begin(); it!=end(); it++)
        {
            if ((*it)->getTagName() == tag->getTagName())
            exist = true;
        }
    }
    if (exist == false)
    {
        tags.push_back(tag);
        setModified();
    }
    cout << tags.first()->getTagName().toStdString() << endl;
    cout << tags.size() << endl;
}


/*!
 * \brief Note::remTag
 * \param tag
 * Cette fonction permet de supprimer le Tag "tag" de la liste des tags de la note
 */
void Note::remTag(Tags *tag)
{
    bool exist = false;
    for(Note::TagIterator it = begin(); it!=end(); it++)
    {
        if ((*it)->getTagName() == tag->getTagName())
        exist = true;
    }
    if (exist == true)
        tags.removeOne(tag);
}

/*!
 * \brief Note::isTagged
 * \param name
 * \return
 * Cette fonction permet de savoir si le Tag nommé "name" est dans la liste des tags de la note
 */
bool Note::isTagged(const QString& name)
{
    bool exist = false;
    for(Note::TagIterator it = begin(); it!=end(); it++)
    {
        if ((*it)->getTagName() == name)
        exist = true;
    }
    return exist;
}

/*!
 * \brief La note est-elle tagguée ?
 * \param name
 * \return
 * Cette méthode permet de savoir si la note possède un Tags qui contient name.
 */
bool Note::isApproxTagged(const QString& name)
{
    bool exist = false;
    for(Note::TagIterator it = begin(); it!=end(); it++)
    {
        if ((*it)->getTagName().contains(name))
        exist = true;
    }
    return exist;
}
/*
QString Note::exportAsPart(ExportStrategy* es, unsigned int levelTitle)
{
    es->exportNote(this, levelTitle);
}
*/
Article::Article(unsigned int id, const QString& ti, const QString& te):
    Note(id, ti), text(te) {}

Article::Article(const Note & note) : Note(note.getID(), note.getTitle())
{

}

/*!
 * \brief Modification de texte
 * \param t
 * Cette fonction modifie le texte d'un article.
 * Elle remplace l'ancien texte par \param t
 */
void Article::setText(const QString& t) {
    isModified=true; text=t;
}

/*!
 * \brief Chargement d'un Article
 * \return
 * Cette fonction défini comment est chargé un article
 */
bool Article::load()
{
    cout << "load"<<endl;
    QString ws = NotesManager::getInstance()->getWorkSpace();
    QString theid;
    theid.setNum(getID());
    ws += theid + ".not";
    cout << ws.toStdString() << endl;
    QFile fichier(ws);

    if (!fichier.open(QIODevice::ReadOnly))
    {
        throw NotesException("Impossible d'ouvrir le fichier");
        return false;
    }
    QTextStream in(&fichier);

    in.readLine();
    in.readLine();
    in.readLine();
    cout << "On a lu les 2 premières lignes" << endl;
    QString texte = in.readAll();
    text = texte;
    fichier.close();
    setLoaded();
    return true;
}

Document::Document(unsigned int id, const QString& t):
    Note(id, t)
{}

Document::Document(const Document &doc) : Note(doc.getID(), doc.getTitle())
{
    /*notes = new QList<Note*>;
   for(unsigned int i = 0; i<doc.getNb(); i++)
       notes.push_back(doc.getNote(i));*/
}

/*!
 * \brief Ajout de note
 * \param note
 * Cette fonction ajoute la note "note" dans la liste des notes du Document
 */
void Document::addNote(Note& note)
{
    notes.push_back(&note);
    setModified();
}

/*!
 * \brief Document::operator <<
 * \param note
 * \return
 * Surcharge de l'opérateur << pour l'ajout des notes
 */
Document& Document::operator<<(Note& note)
{
    addNote(note);
    return(*this);
}

/*!
 * \brief Supprime une note
 * \param i
 * Supprime la note à la place i
 */
void Document::removeNote(unsigned int i)
{
    notes.removeAt(i);
}

/*!
 * \brief Supprime une note
 * \param note
 * Supprime la note "note" du document
 */
void Document::removeNote(Note* note)
{
    if (notes.contains(note))
        notes.removeOne(note);
}


void Document::moveArticleUp(unsigned int i)
{
    if (i != 0)
        notes.swap(i, i-1);
    else
        throw new NotesException("Tentative de monter la premiere note");
}

void Document::moveArticleDown(unsigned int i)
{
    if (i != notes.size()-1)
        notes.swap(i, i+1);
    else
        throw new NotesException("Tentative de descendre la dernière note");
}

/*!
 * \brief Récupère une note
 * \param i
 * \return
 * Cette fonction renvoie la note i du document
 */
Note* Document::getNote(unsigned int i)
{
    if (i>=0 && i<notes.size()){
        return notes[i];
    }else throw NotesException("error, this article does not exists");
}




/*!
 * \brief Chargement d'un document
 * \return
 * Cette fonction défini comment est chargé un document
 */
bool Document::load()
{
    cout << "load"<<endl;
    QString ws = NotesManager::getInstance()->getWorkSpace();
    QString theid;
    theid.setNum(getID());
    ws += theid + ".not";
    cout << ws.toStdString() << endl;
    QFile fichier(ws);

    if (!fichier.open(QIODevice::ReadOnly))
    {
        throw NotesException("Impossible d'ouvrir le fichier");
        return false;
    }
    QTextStream in(&fichier);

    in.readLine();
    in.readLine();
    in.readLine();
    cout << "On a lu les 3 premières lignes" << endl;
    QString nbNote = in.readLine();
    int nbNoteInt = nbNote.toInt();
    for (int i = 0; i < nbNoteInt; i++)
    {
        QString idnote = in.readLine();
        cout << "on ouvre la note " << idnote.toStdString() << endl;
        if (idnote != "")
        {
            unsigned int i = idnote.toInt();
            Note* note = NotesManager::getInstance()->getNote(i);
            notes.append(note);
            cout << "NoEnd" << note << endl;
        }
    }
    cout << "end ?" << endl;
    fichier.close();
    setLoaded();
    return true;
}


/*!
 * \brief Chargement d'un document
 * \return
 * Cette fonction défini comment est chargé un media audio
 */
bool Audio::load()
{
    cout << "load"<<endl;
    QString ws = NotesManager::getInstance()->getWorkSpace();
    QString theid;
    theid.setNum(getID());
    ws += theid + ".not";
    cout << ws.toStdString() << endl;
    QFile fichier(ws);

    if (!fichier.open(QIODevice::ReadOnly))
    {
        throw NotesException("Impossible d'ouvrir le fichier");
        return false;
    }
    QTextStream in(&fichier);

    in.readLine();
    in.readLine();
    in.readLine();
    cout << "On a lu les 2 premières lignes" << endl;
    QString chemin = in.readLine();
    path = chemin;
    QString desc = in.readAll();
    description = desc;
    fichier.close();
    setLoaded();
    return true;
}


/*!
 * \brief Chargement d'un document
 * \return
 * Cette fonction défini comment est chargé un media video
 */
bool Video::load()
{
    cout << "load"<<endl;
    QString ws = NotesManager::getInstance()->getWorkSpace();
    QString theid;
    theid.setNum(getID());
    ws += theid + ".not";
    cout << ws.toStdString() << endl;
    QFile fichier(ws);

    if (!fichier.open(QIODevice::ReadOnly))
    {
        throw NotesException("Impossible d'ouvrir le fichier");
        return false;
    }
    QTextStream in(&fichier);

    in.readLine();
    in.readLine();
    in.readLine();
    cout << "On a lu les 2 premières lignes" << endl;
    QString chemin = in.readLine();
    path = chemin;
    QString desc = in.readAll();
    description = desc;
    fichier.close();
    setLoaded();
    return true;
}

/*!
 * \brief Chargement d'un document
 * \return
 * Cette fonction défini comment est chargé un media image
 */
bool Image::load()
{
    cout << "load"<<endl;
    QString ws = NotesManager::getInstance()->getWorkSpace();
    QString theid;
    theid.setNum(getID());
    ws += theid + ".not";
    cout << ws.toStdString() << endl;
    QFile fichier(ws);

    if (!fichier.open(QIODevice::ReadOnly))
    {
        throw NotesException("Impossible d'ouvrir le fichier");
        return false;
    }
    QTextStream in(&fichier);

    in.readLine();
    in.readLine();
    in.readLine();
    cout << "On a lu les 2 premières lignes" << endl;
    QString chemin = in.readLine();
    path = chemin;
    QString desc = in.readAll();
    description = desc;
    fichier.close();
    setLoaded();
    return true;
}
