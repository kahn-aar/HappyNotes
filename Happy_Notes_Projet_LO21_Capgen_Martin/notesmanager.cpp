#include "notesmanager.h"
#include "notesediteur.h"
#include <fstream>
#include <iostream>
#include <QFile>
#include <QStringList>

NotesManager* NotesManager::instance=0; // pointeur sur l'unique instance

NotesManager::~NotesManager()
{
    for (Iterator it = begin(); it != end(); it++)
    {
        Note* note = *it;
        delete note;
    }
    notes.clear();
    clearBin();
    delete factories["Article"];
    delete factories["Document"];
    delete factories["Image"];
    delete factories["Audio"];
    delete factories["Video"];
    delete[] &factories;
    delete[] &notes;
    delete[] &corbeille;
    delete[] &filteredNotes;
}

/*!
 * \brief NotesManager::getInstance
 * \return l'instance du NotesManager
 *
 * Fonction du design pattern Singleton.
 * Si l'instance n'existe pas, cette fonction la créé, sinon elle renvoie un pointeur sur l'instance.
 */
NotesManager* NotesManager::getInstance(){
    if (!instance) instance=new NotesManager;
    return instance;
}

void NotesManager::libererInstance(){
    if (instance) delete instance;
    instance=0;
}

/*!
 * \brief Constructeur
 *
 * Ce constructeur initialise les attributs MAP du NotesManager, et charge partiellement toutes les notes en mémoire.
 * Il créé de plus le TagsManager avec les Tags sauvegardées.
 * Il va lire le fichier de description se trouvant dans le workspace, le lit, et charge ses données. Il ouvre ensuite chaque fichier de note pour charger partiellement chaque note.
 */
NotesManager::NotesManager() : workspace("../workspace/")
{
    factories = NotesFactory::getFactories();
    strategies = ExportStrategy::getStrategies();
    QFile desc(workspace+"description.desc");
    QStringList listfilename;
    if (!desc.exists())
    {
        desc.open(QIODevice::WriteOnly);
        QTextStream out(&desc);
        out << "0\n0";
        desc.close();
    }
    if (!desc.open(QIODevice::ReadOnly))
        throw NotesException("Erreur de chargement du fichier de descripion");
    QTextStream in(&desc);
    QString nbstr = in.readLine();
    int nb = nbstr.toInt();

    for(int i = 0; i<nb; i++)
    {
        listfilename << in.readLine();
     }
    nbstr = in.readLine();
    nb = nbstr.toInt();
    for (int i = 0; i<nb; i++)
    {
        QString tag = in.readLine();
        TagsManager::getInstance()->addTag(tag);
    }
    for (int i = 0; i<listfilename.size() ; i++)
    {
        QFile note(workspace+listfilename.at(i)+".not");
        if (!note.open(QIODevice::ReadOnly))
            throw NotesException("Erreur de chargement du fichier");
        QTextStream in2(&note);
        QString type = in2.readLine();
        QString tags = in2.readLine();


        QString title = in2.readLine();
        Note* newNote = getNote(listfilename.at(i).toInt(), type, title);

        if (tags != "NULL")

        {
             tags.remove(tags.size()-1, 1);
            QStringList listTags = tags.split(",");
            cout<< listTags.at(0).toStdString() <<endl;
            for(int j = 0; j<listTags.size(); j++)
            {
                newNote->addTag(TagsManager::getInstance()->getTag(listTags.at(j)));
            }
        }
        newNote->setNoModified();
        notes.insert(newNote);
        note.close();
    }
    desc.close();
    for (NotesManager::Iterator it = begin(); it != end(); it++)
    {
        filteredNotes.insert(*it);
    }
}

/*!
 * \brief Modifie le dossier WorkSpace
 * \param ws : nom du nouveau WorkSpace
 *
 *
 */
void NotesManager::setWorkspace(const QString& ws)
{
    workspace = ws+"/";
    notes.clear();
    filteredNotes.clear();
    corbeille.clear();
    QFile description(ws + "description.desc");
    description.open(QIODevice::WriteOnly);
    QTextStream in(&description);
    in << "0\n0";
    description.close();
    NotesEditeur::getInstance()->refreshModel();
}

/*!
 * \brief Modifie le dossier WorkSpace
 * \param ws : chemin du nouveau dossier Workspace
 *
 * Cette méthode copie toutes les notes dans le nouveau workspace
 */
void NotesManager::setCopyWorkspace(const QString& ws)
{
    QFile newDesc(ws + "/description.desc");
    newDesc.open(QIODevice::WriteOnly);
    QTextStream in(&newDesc);
    QFile oldDesc(workspace + "description.desc");
    oldDesc.open(QIODevice::ReadOnly);
    QTextStream out(&oldDesc);
    QString strNb = out.readLine();
    in << strNb << "\n";
    int nb = strNb.toInt();
    for(int i = 0; i<nb; i++)
    {
        QString ID = out.readLine();
        in << ID << "\n";
        QFile newNote(ws + "/" + ID + ".not");
        newNote.open(QIODevice::WriteOnly);
        QTextStream in2(&newNote);
        QFile oldNote(workspace + ID + ".not");
        oldNote.open(QIODevice::ReadOnly);
        QTextStream out2(&oldNote);
        in2 << out2.readAll();
        newNote.close();
        oldNote.close();
    }
    while (out.atEnd())
    {
        in << out.readLine() << "\n";
    }
    newDesc.close();
    oldDesc.close();


    workspace = ws + "/";
}


/*!
 * \brief Change le Workspace et charge les notes de celui-ci
 * \param desc : chemin du fichier de description
 */
void NotesManager::setChangeWorkspace(QString& desc)
{
    notes.clear();
    filteredNotes.clear();
    corbeille.clear();
    TagsManager::getInstance()->clear();
    desc.resize(desc.size()-16);
    workspace = desc;
    cout << workspace.toStdString() << endl;
    QStringList listfilename;
    QFile description(workspace + "description.desc");
    if (!description.open(QIODevice::ReadOnly))
        throw NotesException("Erreur dans l'ouverture du fichier.");

    QTextStream in(&description);

    QString nbstr = in.readLine();
    int nb = nbstr.toInt();

    for(int i = 0; i<nb; i++)
    {
        listfilename << in.readLine();
        cout << "lecture" << endl;
     }
    nbstr = in.readLine();
    nb = nbstr.toInt();
    for (int i = 0; i<nb; i++)
    {
        QString tag = in.readLine();
        TagsManager::getInstance()->addTag(tag);
        cout << "fin tag" << endl;
    }
    for (int i = 0; i<listfilename.size() ; i++)
    {
        cout << workspace.toStdString() << listfilename.at(i).toStdString() << ".not" << endl;
        QFile note(workspace+listfilename.at(i)+".not");
        if (!note.open(QIODevice::ReadOnly))
            throw NotesException("Erreur de chargement du fichier");
        QTextStream in2(&note);
        QString type = in2.readLine();
        QString tags = in2.readLine();
        QString title = in2.readLine();
        Note* newNote = getNote(listfilename.at(i).toInt(), type, title);

        if (tags != "NULL")

        {
             tags.remove(tags.size()-1, 1);
             cout << tags.toStdString() << endl;
            QStringList listTags = tags.split(",");
            for(int j = 0; j<listTags.size(); j++)
            {
                newNote->addTag(TagsManager::getInstance()->getTag(listTags.at(j)));
            }
        }
        newNote->setNoModified();
        notes.insert(newNote);
        note.close();
    }
    description.close();
    for (NotesManager::Iterator it = begin(); it != end(); it++)
    {
        filteredNotes.insert(*it);
    }
    NotesEditeur::getInstance()->refreshModel();
}

/*!
 * \brief Exporte la note dans le format voulu
 * \param strat : nom du format
 *
 * Cette méthode va appeller la bonne strategy afin d'exporter la note.
 */
void NotesManager::exporterNote(const QString& strat, Note* n)
{
    QString type = getType(*n);
    if (type == "Article"){
        Article* note = dynamic_cast<Article*>(n);
        strategies[strat]->header(note);
        strategies[strat]->exportNote(note,1);
        strategies[strat]->footer(note);
    }
    if (type == "Document"){
         Document* note = dynamic_cast<Document*>(n);
         strategies[strat]->header(note);
         strategies[strat]->exportNote(note, 1);
         strategies[strat]->footer(note);
     }
    if (type == "Image"){
         Image* note = dynamic_cast<Image*>(n);
         strategies[strat]->header(note);
         strategies[strat]->exportNote(note,1);
         strategies[strat]->footer(note);
     }
    if (type == "Audio"){
         Audio* note = dynamic_cast<Audio*>(n);
         strategies[strat]->header(note);
         strategies[strat]->exportNote(note,1);
         strategies[strat]->footer(note);
     }
    if (type == "Video") {
        Video* note = dynamic_cast<Video*>(n);
        strategies[strat]->header(note);
        strategies[strat]->exportNote(note,1);
        strategies[strat]->footer(note);
    }
}

/*!
 * \brief Création d'une note
 *
 * getNewNote va appeler la bonne factory pour créer une nouvelle note.
 */
Note* NotesManager::getNewNote(const QString& type, const QString& title)
{
    Note* note = factories[type]->buildNewNote(title);
    notes.insert(note);
    filter(NotesEditeur::getInstance()->getFilter());
    note->setLoaded();
    return note;
}

/*!
 * \brief Ouverture d'une note
 *
 * getNewNote va appeler la bonne factory pour ouvrir une note déjà existante (Dont le fichier existe).
 */
Note* NotesManager::getNote(unsigned int id, const QString& type, const QString& title)
{
    Note* note = factories[type]->buildNote(id,title);
    return note;
}

/*!
 * \brief Récupération d'une note
 * \param titre
 * \return
 * Cette méthode renvoye la note dont le titre est "titre".
 */
Note* NotesManager::getNote(const QString& titre)
{
    for(Iterator it = begin(); it != end(); it++ )
        if( (*it)->getTitle() == titre )
            return (*it);
        return 0;
}

/*!
 * \brief Récupération d'une note
 * \param i
 * \return
 * Cette méthode renvoye la note avec l'ID i.
 */
Note* NotesManager::getNote(unsigned int i)
{
    for(Iterator it = begin(); it != end(); it++ )
        if( (*it)->getID() == i )
            return (*it);
        return 0;
}

void NotesManager::supprNote(Note* note)
{
    for (Iterator it = begin(); it != end(); it++)
    {
        if (getType(*(*it)) == "Document")
        {
            Document* doc = dynamic_cast<Document*>(*it);
            doc->removeNote(note);
            if (doc->getScreen())
                NotesEditeur::getInstance()->load_document(doc);
        }
    }
    notes.remove(note);
}

/*!
 * \brief Récupération d'une note
 * Cette méthode renvoye la note contenue dans la corbeille dont le titre est "titre".
 */
Note* NotesManager::getBinNote(const QString& titre)
{
    for(Iterator it = begin_bin(); it != end_bin(); it++ )
        if( (*it)->getTitle() == titre )
            return (*it);
        return 0;
}

/*!
 * \brief vérifie si une note est dans la corbeille
 */
bool NotesManager::isInBin(Note* note)
{
    return corbeille.contains(note);
}

/*!
 * \brief Sauvegarde des notes
 *
 * Cette méthode sauvegarde toutes les notes dans les fichiers.
 * Elle ouvre et écrit dans le fichier de description, et enregistre chaque note dans leur fichier, si elles ont été modifiées. Sinon, elle ne sont pas ré-enregistrées.
 * Chaque type de Note est enregistrés différament : Les Articles ont leur texte en dessous des Tags, Les documents ont l'ID de chaque Note qu'elle contient et les médias ont leur chemin puis leur description.
 */
void NotesManager::saveNotes()
{
    QFile description(workspace+"description.desc");
    description.open(QIODevice::WriteOnly);
    QTextStream wte(&description);
    unsigned int nb = getNbNotes();
    QString nbStr;
    nbStr.setNum(nb);
    wte << nbStr << "\n";
    for (Iterator it = begin(); it != end(); it++)
    {
        unsigned int i = (*it)->getID();
        QString namefile(getWorkSpace()) ;
        QString IDFile;
        IDFile.setNum(i);
        wte << IDFile << "\n";
        if ((*it)->getModified())
        {
            QFile fichier(namefile+IDFile+".not");
            if (!fichier.open(QIODevice::WriteOnly))
                throw NotesException("Erreur de chargement du fichier");
            fichier.flush();
            QTextStream out(&fichier);

            out << getType(*(*it))<< "\n";

            if ((*it)->getSize() == 0)
            {
                out << "NULL";
            }
            else
                for (Note::TagIterator tagit = (*it)->begin(); tagit != (*it)->end(); tagit++)
                {
                    out << (*tagit)->getTagName() << ",";
                }
            out << "\n";

            QString titre = (*it)->getTitle();


            if(getType(*(*it)) == "Article")
            {
                Article* art = dynamic_cast<Article*>(*it);
                out << titre << "\n";
                out << art->getText();
            }
            if(getType(*(*it)) == "Document")
            {
                Document* doc = dynamic_cast<Document*>(*it);
                out << titre << "\n";
                QString nb;
                nb.setNum(doc->getNb());
                out << nb << "\n";
                for (Document::Iterator itdoc = doc->begin(); itdoc != doc->end(); itdoc++)
                {
                    QString theid;
                    theid.setNum((*itdoc)->getID());

                    out << theid << "\n";

                }
            }
            if(getType(*(*it)) == "Image")
            {
                Image* img = dynamic_cast<Image*>(*it);
                out << titre << "\n";
                out << img->getPath() << "\n";
                out << img->getDesc();
            }
            if(getType(*(*it)) == "Video")
            {
                Video* video = dynamic_cast<Video*>(*it);
                out << titre << "\n";
                out << video->getPath() << "\n";
                out << video->getDesc();
            }
            if(getType(*(*it)) == "Audio")
            {
                Audio* audio = dynamic_cast<Audio*>(*it);
                out << titre << "\n";
                out << audio->getPath() << "\n";
                out << audio->getDesc();
            }
            (*it)->setNoModified();

            fichier.close();

        }
    }
    nb = TagsManager::getInstance()->getSize();
    nbStr.setNum(nb);
    wte << nbStr << "\n";
    for (TagsManager::Iterator it = TagsManager::getInstance()->begin(); it != TagsManager::getInstance()->end(); it++)
    {
        wte << (*it)->getTagName() << "\n";

    }
    description.close();
    NotesEditeur::getInstance()->refreshModel();
}


/*!
 * \brief Initialisation de la liste filtrée
 *
 * Initialise la liste filtrée en copiant la liste des notes dans la liste filtrée.
 */
void NotesManager::initializeFiltered()
{
    for (NotesManager::Iterator it = begin(); it != end(); it++)
    {
        filteredNotes.insert(*it);
    }
}

/*!
 * \brief Filtre les notes
 * \param filtre
 *
 * Cette méthode filtre les notes selon les tags nommées "filtre". Le résultat se retrouve dans la liste filtrée.
 */
void NotesManager::filter(const QString& filtre)
{
    deleteFiltered();
    if (filtre == "")
        initializeFiltered();
    else
        for (NotesManager::Iterator it = begin(); it != end(); it++)
        {
            if ((*it)->isTagged(filtre))
             filteredNotes.insert(*it);
        }
}

/*!
 * \brief Filtre les notes
 * \param filtre
 *
 * Cette méthode filtre les notes selon les tags qui contienent le filtre. Le résultat se retrouve dans la liste filtrée.
 */
void NotesManager::filter_content(const QString& filtre)
{
    deleteFiltered();
    if (filtre == "")
        initializeFiltered();
    else
        for (NotesManager::Iterator it = begin(); it != end(); it++)
        {
            if ((*it)->isApproxTagged(filtre))
             filteredNotes.insert(*it);
        }
}

/*!
 * \brief Jette une note à la corbeille
 * \param note
 *
 * Cette méthode insère la note dans la liste corbeille et la supprime de la liste de notes (ainsi que de la liste filtrée).
 */
void NotesManager::trash(Note* note)
{
    corbeille.insert(note);
    supprNote(note);
    filteredNotes.remove(note);
}

/*!
 * \brief Restaure la note
 * \param note
 * \param filtre
 *
 * Cette méthode insère la note dans la liste de notes et la supprime de la liste corbeille. Elle met à jour la liste filtrée.
 */
void NotesManager::restaure(Note *note, const QString& filtre)
{
    notes.insert(note);
    corbeille.remove(note);
    filter(filtre);
}

/*!
 * \brief Restaures toutes les notes
 * \param filtre
 *
 * Cette méthode insère les notes dans la liste de notes et les supprimes de la liste corbeille. Elle met à jour la liste filtrée.
 */
void NotesManager::restaureAll(const QString& filtre)
{
    for (NotesManager::Iterator it = NotesManager::getInstance()->begin_bin(); it != NotesManager::getInstance()->end_bin(); it++)
    {
        notes.insert( *it);
    }
    corbeille.clear();
    filter(filtre);
}

/*!
 * \brief Détruit la note
 * \param note
 *
 * Cette méthode détruit la note de la corbeille. Elle la retire de la liste puis la supprime.
 */
void NotesManager::destroy(Note* note)
{
    corbeille.remove(note);
    delete note;
}

/*!
 * \brief Détruit toutes les notes de la corbeille
 *
 * Cette méthode vide la corbeille. Elle supprime toutes les notes contenues dans la corbeille.
 */
void NotesManager::clearBin()
{
    for (NotesManager::Iterator it = NotesManager::getInstance()->begin_bin(); it != NotesManager::getInstance()->end_bin(); it++)
    {
        delete *it;
    }
    corbeille.clear();
}

/*!
 * \brief Retire un Tags de toutes les notes
 * \param tag
 */
void NotesManager::removeTag(Tags* tag)
{
    for(Iterator it = begin(); it != end(); it++)
    {
        if ((*it)->isTagged(tag->getTagName()))
        {
            (*it)->remTag(tag);
        }
    }
}

/*!
 * \brief Renvoye le type de la note
 * \param note
 * \return
 *
 * Cette méthode permet de récupérer le type de la note.
 */
const QString NotesManager::getType(const Note& note) const
{
    QString type = "Media";
     const Media* test = dynamic_cast< const Media*>( &note );
    if( !test ) { const Article* test2 = dynamic_cast< const Article*>( &note );
        if( !test2 )type = "Document";
        else type = "Article";
        }
    else{
        const Audio* test3 = dynamic_cast< const Audio*>( &note );
        if( !test3 ){
            const Image* test4 = dynamic_cast< const Image*>( &note );
            if(!test4) type = "Video";
            else type = "Image";
        }
        else type = "Audio";
    }

    return type;
}
