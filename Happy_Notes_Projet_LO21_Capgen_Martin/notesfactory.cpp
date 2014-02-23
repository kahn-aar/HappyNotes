#include "notesfactory.h"
using namespace std;


unsigned int NotesFactory::newId() const
{

    time_t timer;
    struct tm y2k;
    double seconds;

    y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
    y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

    time(&timer);  /* get current time; same as: timer = time(NULL)  */

    seconds = difftime(timer,mktime(&y2k));

    return seconds;
}

QMap<QString, NotesFactory*> NotesFactory::getFactories()
{
    QMap<QString, NotesFactory*> factories;
    factories.insert("Article",new ArticleFactory);
    factories.insert("Document",new DocumentFactory);
    factories.insert("Audio",new AudioFactory);
    factories.insert("Image",new ImageFactory);
    factories.insert("Video",new VideoFactory);
    return factories;
}

/*!
 * \brief Construit un Article dont on possède déjà l'ID.
 * \param ID
 * \param title
 * \return
 */
Note* ArticleFactory::buildNote(unsigned int ID, QString title) const
{
    Note* art = new Article(ID, title);
    return art;
}

/*!
 * \brief Construit un nouvel article. On lui affecte un ID généré.
 * \param title
 * \return
 */
Note* ArticleFactory::buildNewNote(QString title) const
{
    cout << "begin of creation"<<endl;
    unsigned int ID = NotesFactory::newId();
    Note* art = new Article(ID, title);
    return art;
}

/*!
 * \brief Construit une copie d'un Article
 * \param note
 * \return
 */
Note *ArticleFactory::buildNoteCopy(const Note* note) const
{
    Note* art = new Article(*note);
    return art;
}

/*!
 * \brief Construit un Document dont on possède déjà l'ID.
 * \param ID
 * \param title
 * \return
 */
Note* DocumentFactory::buildNote(unsigned int id, QString title) const
{
    Note* doc = new Document(id, title);
    return doc;
}

/*!
 * \brief Construit un nouveau Document. On lui affecte un ID généré.
 * \param title
 * \return
 */
Note* DocumentFactory::buildNewNote(QString title) const
{
    unsigned int ID = NotesFactory::newId();
    Note* doc = new Document(ID, title);
    return doc;
}

/*!
 * \brief Construit une copie d'un Document
 * \param note
 * \return
 */
Note* DocumentFactory::buildNoteCopy(const Note* note) const
{
    Note* doc = new Document(*note);
    return doc;
}

/*!
 * \brief Construit une Image dont on possède déjà l'ID.
 * \param ID
 * \param title
 * \return
 */
Note* ImageFactory::buildNote(unsigned int id, QString title)const
{
    Note* img = new Image(id, title);
    return img;
}

/*!
 * \brief Construit une nouvelle Image. On lui affecte un ID généré.
 * \param title
 * \return
 */
Note* ImageFactory::buildNewNote(QString title)const
{
    unsigned int ID = NotesFactory::newId();
    Note* img = new Image(ID, title);
    return img;
}

/*!
 * \brief Construit une copie d'une Image
 * \param note
 * \return
 */
Note* ImageFactory::buildNoteCopy(const Note* note)const
{
    Note* img = new Image(*note);
    return img;
}

/*!
 * \brief Construit une Note Audio dont on possède déjà l'ID.
 * \param ID
 * \param title
 * \return
 */
Note* AudioFactory::buildNote(unsigned int id, QString title) const
{
    Note* aud = new Audio(id, title);
    return aud;
}

/*!
 * \brief Construit un nouvelle Note Audio. On lui affecte un ID généré.
 * \param title
 * \return
 */
Note* AudioFactory::buildNewNote(QString title) const
{
    unsigned int ID = NotesFactory::newId();
    Note* aud = new Audio(ID, title);
    return aud;
}

/*!
 * \brief Construit une copie d'une Note Audio
 * \param note
 * \return
 */
Note* AudioFactory::buildNoteCopy(const Note* note) const
{
    Note* aud = new Audio(*note);
    return aud;
}

/*!
 * \brief Construit une Video dont on possède déjà l'ID.
 * \param ID
 * \param title
 * \return
 */
Note* VideoFactory::buildNote(unsigned int id, QString title) const
{
    Note* vid = new Video(id, title);
    return vid;
}

/*!
 * \brief Construit un nouvelle Note Video. On lui affecte un ID généré.
 * \param title
 * \return
 */
Note* VideoFactory::buildNewNote(QString title) const
{
    unsigned int ID = NotesFactory::newId();
    Note* vid = new Video(ID, title);
    return vid;
}

/*!
 * \brief Construit une copie d'une Note Video
 * \param note
 * \return
 */
Note* VideoFactory::buildNoteCopy(const Note* note) const
{
    Note* vid = new Video(*note);
    return vid;
}
