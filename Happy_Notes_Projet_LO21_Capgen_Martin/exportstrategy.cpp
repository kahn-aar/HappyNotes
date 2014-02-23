#include "exportstrategy.h"
#include <QDir>

/*!
 * \brief Récupère les stratégies
 * \return : la QMap renvoyée fera correspondre une QString (nom de l'export) et une strategy.
 */
QMap<QString, ExportStrategy *> ExportStrategy::getStrategies()
{
    QMap<QString, ExportStrategy*> strategies;
    strategies.insert("HTML",new HTMLExport);
    strategies.insert("TeX",new TeXExport);
    strategies.insert("Texte",new TextExport);
    return strategies;
}

/*!
 * \brief Exporte le header HTML d'une note.
 * \param n : La note à exporter
 */
void HTMLExport::header(const Note *n) const
{
    QDir exports(NotesManager::getInstance()->getWorkSpace() + "/Export");
    if (!exports.exists())
    {
        QDir ws(NotesManager::getInstance()->getWorkSpace());
        ws.mkdir("Export");
    }
    QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +n->getTitle() +".html");
    description.open(QIODevice::WriteOnly);
    QTextStream wte(&description);

    wte << "<html>" << "\n";
    wte << "\t" << "<head>" << "\n";
    wte << "\t<meta charset = \"UTF-8\">" << "\n";
    wte << "\t<title>" << n->getTitle() << "</title>" << "\n";
    wte << "\t</head>" << "\n";
    wte << "<body>" << "\n";
    description.close();

}

/*!
 * \brief Exporte le header LaTeX d'une note.
 * \param n : La note à exporter
 */
void TeXExport::header(const Note *n) const
{
    QDir exports(NotesManager::getInstance()->getWorkSpace() + "/Export");
    if (!exports.exists())
    {
        QDir ws(NotesManager::getInstance()->getWorkSpace());
        ws.mkdir("Export");
    }
    QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +n->getTitle() +".tex");
    description.open(QIODevice::WriteOnly);
    QTextStream wte(&description);

    wte << "\\documentclass[a4paper,11pt]{report}" << "\n";
    wte << "\\usepackage{graphicx}" << "\n";
    wte << "\\usepackage[french]{babel}" << "\n";
    wte << "\\usepackage[latin1]{inputenc}" << "\n";
    wte << "\\usepackage[T1]{fontenc}" << "\n";
    wte << "\\begin{document}" << "\n";
    description.close();

}


/*!
 * \brief Exporte le header Texte d'une note.
 * \param n : La note à exporter
 */
void TextExport::header(const Note *n) const
{
    QDir exports(NotesManager::getInstance()->getWorkSpace() + "/Export");
    if (!exports.exists())
    {
        QDir ws(NotesManager::getInstance()->getWorkSpace());
        ws.mkdir("Export");
    }
}

/*!
 * \brief Exporte le footer HTML d'une note.
 * \param n : La note à exporter
 */
void HTMLExport::footer(const Note *n) const
{
    QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +n->getTitle() +".html");
    description.open(QIODevice::WriteOnly| QIODevice::Append);
    QTextStream wte(&description);
    wte << "</body>" << "\n";
    wte << "<footer>" <<endl;
    wte << "<p> Créé par : Capgen Léa et Martin Nicolas</p>" <<endl;
    wte << "</footer>" <<endl;
    wte << "</html>";
    description.close();
}

/*!
 * \brief Exporte le footer LaTeX d'une note.
 * \param n : La note à exporter
 */
void TeXExport::footer(const Note *n) const
{
     QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +n->getTitle() +".tex");
    description.open(QIODevice::WriteOnly| QIODevice::Append);
    QTextStream wte(&description);
    wte <<"\\end{document}" <<endl;
    description.close();
}

/*!
 * \brief Exporte le footer Texte d'une note.
 * \param n : La note à exporter
 */
void TextExport::footer(const Note *n) const
{

}

/*!
 * \brief Exporte un Article en HTML
 * \param A
 * \param titleLevel
 */
void HTMLExport::exportNote(const Article* A, unsigned int titleLevel) const
{
    QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +A->getTitle() +".html");
    if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
        throw NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);

    wte << "\t <h" << titleLevel <<">" << A->getTitle()<< "</h" << titleLevel <<"> "<< endl;
    wte << "\t <p>"<< A->getText() << " </p>" <<endl;
    description.close();
}

/*!
 * \brief Exporte un Article en LaTeX
 * \param A
 * \param titleLevel
 */
void TeXExport::exportNote(const Article* A, unsigned int titleLevel) const
{
     QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +A->getTitle() +".tex");
     if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
         throw NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);

    wte << "\\chapter{"<< A->getTitle()<<"}" << endl;
    wte << "\\paragraph{"<< A->getTitle()<<"} " << A->getText() <<endl;
    description.close();
}

/*!
 * \brief Exporte un Article en texte
 * \param A
 * \param titleLevel
 */
void TextExport::exportNote(const Article* A, unsigned int titleLevel) const
{
     QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +A->getTitle() +".txt");
     if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
         throw new NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);

    wte << A->getTitle() << endl;
    wte << A->getText() <<endl;
    description.close();
}

/*!
 * \brief Exporte un Document en HTML.
 * \param A
 * \param titleLevel
 *
 * Cette fonction fait un appel de exportAsPart pour chaque Note contenues dans le Document
 */
void HTMLExport::exportNote(Document* D, unsigned int titleLevel) const
{
    QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +D->getTitle() +".html");
    if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
        throw NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);

    wte << "\t<h" << titleLevel << ">" << D->getTitle()<< "</h" << titleLevel << "> "<< endl;
    description.close();
    for (Document::Iterator it = D->begin(); it != D->end(); it++)
    {
        exportAsPart(*it, 2, NotesManager::getInstance()->getWorkSpace() + "/Export/" +D->getTitle() +".html");
    }
}

/*!
 * \brief Exporte une Note contenue dans un Document
 * \param N : La Note à exporter
 * \param titleLevel : Niveau de la note dans la hierarchie
 * \param filename : chemin du fichier du Document
 *
 * Cette méthode ouvre le fichier, et y ajoute les données de la note selon son type. Elle est appelée à nouveau dans le cas où la Note est elle même un Document.
 */
void HTMLExport::exportAsPart(Note* N, unsigned int titleLevel, const QString& filename) const
{
    QFile description(filename);
    if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
        throw NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);
    wte << "\t<h" << titleLevel << ">" << N->getTitle()<< "</h" << titleLevel << "> "<< endl;
    QString type = NotesManager::getInstance()->getType(*N);
    if (type == "Article")
    {
        Article* A = dynamic_cast<Article*>(N);
        for (unsigned int i = 0; i<titleLevel; i++)
            wte << "\t";
        wte << "<p>"<< A->getText() << " </p>" <<endl;
        description.close();
    }
    if (type == "Image")
    {
        Image* I = dynamic_cast<Image*>(N);
        for (unsigned int i = 0; i<titleLevel; i++)
            wte << "\t";
        wte << "<p>\n";
        for (unsigned int i = 0; i<titleLevel+1; i++)
            wte <<"\t";
        wte << "<img src=\"file:///"<< I->getPath( )<<"\" title=\""<<I->getTitle();
        if (I->getDesc() != "")
            wte << " alt=\"" << I->getDesc() << "\" />" <<endl;
        else
            wte << " alt=\"No description\"/>"  <<endl;
        for (unsigned int i = 0; i<titleLevel; i++)
            wte << "\t";
        wte << "</p>\n";
        description.close();
    }
    if (type == "Audio")
    {
        Audio* A = dynamic_cast<Audio*>(N);
        for (unsigned int i = 0; i<titleLevel; i++)
            wte << "\t";
        wte << "<audio src=\"file:///"<<A->getPath()<<"\" controls /> "<<endl;
        for (unsigned int i = 0; i<titleLevel+1; i++)
            wte << "\t";
        wte << "<a href=\"file:///"<<A->getPath()<<"\">Download song</a> "<<endl;
        for (unsigned int i = 0; i<titleLevel; i++)
            wte << "\t";
        wte << "</audio> "<<endl;
        /*wte << " <audio controls> "<<endl;
        wte << "<source src=\"file:///" << A->getPath()<<"\" /> "<<endl;
        wte << "<p>" << A->getDesc() << "</p> "<<endl;
        wte << "</audio> "<<endl;*/
        description.close();
    }
    if (type == "Video")
    {
        Video* V = dynamic_cast<Video*>(N);
        for (unsigned int i = 0; i<titleLevel; i++)
            wte << "\t";
        wte << " <video controls> " <<endl;
        for (unsigned int i = 0; i<titleLevel+1; i++)
            wte << "\t";
        wte << "<source src=\"file:///" << V->getPath() << " \" type='video/webm; codecs=\"vp8.0, vorbis\"'/> " <<endl;
        for (unsigned int i = 0; i<titleLevel+1; i++)
            wte << "\t";
        wte << "<source src=\"file:///"<< V->getPath() <<" \" type='video/ogg; codecs=\"theora, vorbis\"'/> " <<endl;
        for (unsigned int i = 0; i<titleLevel+1; i++)
            wte << "\t";
        wte << "<source src=\"file:///"<< V->getPath() <<" \" type='video/mp4; codecs=\"avc1.4D401E, mp4a.40.2\"'/> " <<endl;
        for (unsigned int i = 0; i<titleLevel+1; i++)
            wte << "\t";
        wte << "<p>" << V->getDesc() << "</p> " <<endl;
        for (unsigned int i = 0; i<titleLevel; i++)
            wte << "\t";
        wte << "</video> " <<endl;
        description.close();
    }
    if (type == "Document")
    {
        Document* D = dynamic_cast<Document*>(N);
        description.close();
        for(Document::Iterator it = D->begin(); it != D->end(); it++)
        {
            exportAsPart(*it, titleLevel+1, filename);
        }
    }

}

/*!
 * \brief Exporte un Document en LaTeX.
 * \param A
 * \param titleLevel
 *
 * Cette fonction fait un appel de exportAsPart pour chaque Note contenues dans le Document
 */
void TeXExport::exportNote(Document* D, unsigned int titleLevel) const
{
    QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +D->getTitle() +".tex");
    if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
        throw NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);

    wte << "\\chapter{"<<D->getTitle()<<"}" << endl;

    description.close();
    for (Document::Iterator it = D->begin(); it != D->end(); it++)
    {
        exportAsPart(*it, 2, NotesManager::getInstance()->getWorkSpace() + "/Export/" +D->getTitle() +".tex");
    }
}

/*!
 * \brief Exporte une Note contenue dans un Document
 * \param N : La Note à exporter
 * \param titleLevel : Niveau de la note dans la hierarchie
 * \param filename : chemin du fichier du Document
 *
 * Cette méthode ouvre le fichier, et y ajoute les données de la note selon son type. Elle est appelée à nouveau dans le cas où la Note est elle même un Document.
 */
void TeXExport::exportAsPart(Note* N, unsigned int titleLevel, const QString& filename) const
{
    QFile description(filename);
    if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
        throw new NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);
    if (titleLevel == 2)
        wte << "\\section{" << N->getTitle() << "}"<< endl;
    if (titleLevel == 3)
        wte << "\\subsection{" << N->getTitle() << "}"<< endl;
    QString type = NotesManager::getInstance()->getType(*N);
    if (type == "Article")
    {
        Article* A = dynamic_cast<Article*>(N);
        wte << "\\paragraph{Article}"<< A->getText() <<endl;
        description.close();
    }
    if (type == "Image")
    {
        Image* I = dynamic_cast<Image*>(N);
        wte << "\\paragraph{Lien de l'image'}"<< I->getPath() <<endl;
        wte << "\\begin{center}" <<endl;
        wte << "\\includegraphics{" << I->getPath() << "}" <<endl;
        wte << "\\end{center}" <<endl;
        wte << "\\paragraph{Description}"<< I->getDesc() << endl;

        description.close();
    }
    if (type == "Audio")
    {
        Audio* A = dynamic_cast<Audio*>(N);
        wte << "\\paragraph{Lien vidéo}"<< A->getPath() <<endl;
        wte << "\\paragraph{Description}"<< A->getDesc() << endl;
        description.close();
    }
    if (type == "Video")
    {
        Video* V = dynamic_cast<Video*>(N);
        wte << "\\paragraph{Lien vidéo}"<< V->getPath() <<endl;
        wte << "\\paragraph{Description}"<< V->getDesc() << endl;
        description.close();
    }
    if (type == "Document")
    {
        Document* D = dynamic_cast<Document*>(N);
        description.close();
        for(Document::Iterator it = D->begin(); it != D->end(); it++)
        {
            exportAsPart(*it, titleLevel+1, filename);
        }
    }

}


/*!
 * \brief Exporte un Document en Texte.
 * \param A
 * \param titleLevel
 *
 * Cette fonction fait un appel de exportAsPart pour chaque Note contenues dans le Document
 */
void TextExport::exportNote(Document* D, unsigned int titleLevel) const
{
    QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +D->getTitle() +".txt");
    if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
        throw NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);

    wte << D->getTitle() <<endl;
    description.close();
    for(Document::Iterator it = D->begin(); it != D->end(); it++)
    {
        exportAsPart(*it, 2, NotesManager::getInstance()->getWorkSpace() + "/Export/" +D->getTitle() +".txt");
    }

}

/*!
 * \brief Exporte une Note contenue dans un Document
 * \param N : La Note à exporter
 * \param titleLevel : Niveau de la note dans la hierarchie
 * \param filename : chemin du fichier du Document
 *
 * Cette méthode ouvre le fichier, et y ajoute les données de la note selon son type. Elle est appelée à nouveau dans le cas où la Note est elle même un Document.
 */
void TextExport::exportAsPart(Note* N, unsigned int titleLevel, const QString& filename) const
{
    QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +N->getTitle() +".txt");
    if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
        throw NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);

    wte << "\t" << N->getTitle() << endl;
    QString type = NotesManager::getInstance()->getType(*N);
    if (type == "Article")
    {
        Article* A = dynamic_cast<Article*>(N);
        wte << A->getText() <<endl;
        description.close();
    }
    if (type == "Image")
    {
        Image* I = dynamic_cast<Image*>(N);
        wte << I->getPath() <<endl;
        wte << I->getDesc() << endl;

        description.close();
    }
    if (type == "Audio")
    {
        Audio* A = dynamic_cast<Audio*>(N);
        wte << A->getPath() <<endl;
        wte << A->getDesc() << endl;
        description.close();
    }
    if (type == "Video")
    {
        Video* V = dynamic_cast<Video*>(N);
        wte << V->getPath() <<endl;
        wte << V->getDesc() << endl;
        description.close();
    }
    if (type == "Document")
    {
        Document* D = dynamic_cast<Document*>(N);
        description.close();
        for(Document::Iterator it = D->begin(); it != D->end(); it++)
        {
            exportAsPart(*it, titleLevel+1, filename);
        }
    }

}

/*!
 * \brief Exporte une Image en HTML
 * \param I
 * \param titleLevel
 */
void HTMLExport::exportNote(const Image* I, unsigned int titleLevel) const
{
    QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +I->getTitle() +".html");
    if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
        throw NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);
    wte << "\t<h" << titleLevel << ">" << I->getTitle() << "</h" << titleLevel << ">";
    wte << "\t<p>\n";
    wte << "\t\t<img src=\"file:///"<< I->getPath( )<<"\" title=\""<<I->getTitle();
    if (I->getDesc() != "")
        wte << " alt=\"" << I->getDesc() << "\"/>" <<endl;
    else
        wte << " alt=\"No description\""  <<endl;
    wte << "\t</p>\n";
    description.close();
}

/*!
 * \brief Exporte une Image en LaTeX
 * \param I
 * \param titleLevel
 */
void TeXExport::exportNote(const Image* I, unsigned int titleLevel) const
{
    QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +I->getTitle() +".tex");
    if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
        throw NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);

    wte << "\\chapter{"<< I->getTitle()<<"}" << endl;
    wte << "\\paragraph{Lien de l'image'}"<< I->getPath() <<endl;
    wte << "\\begin{center}" <<endl;
    wte << "\\includegraphics{" << I->getPath() << "}" <<endl;
    wte << "\\end{center}" <<endl;
    wte << "\\paragraph{Description}"<< I->getDesc() << endl;

    description.close();
}

/*!
 * \brief Exporte une Image en Texte
 * \param I
 * \param titleLevel
 */
void TextExport::exportNote(const Image* I, unsigned int titleLevel) const
{
    QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +I->getTitle() +".txt");
    if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
        throw NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);

    wte << I->getTitle() <<endl;
    wte << I->getPath() <<endl;
    description.close();
}

/*!
 * \brief Exporte un fichier Audio en HTML
 * \param A
 * \param titleLevel
 */
void HTMLExport::exportNote(const Audio* A, unsigned int titleLevel) const
{
    QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +A->getTitle() +".html");
    if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
        throw NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);


    wte << "<audio src=\"file:///"<<A->getPath()<<"\" controls /> "<<endl;
    wte << "<a href=\"file:///"<<A->getPath()<<"\">Download song</a> "<<endl;
    wte << "</audio> "<<endl;
    wte << " <audio controls> "<<endl;
    wte << "<source src=\"file:///" << A->getPath()<<"\" /> "<<endl;
    wte << "<p>" << A->getDesc() << "</p> "<<endl;
    wte << "</audio> "<<endl;
    description.close();
}

/*!
 * \brief Exporte un fichier Audio en LaTeX
 * \param A
 * \param titleLevel
 */
void TeXExport::exportNote(const Audio* A, unsigned int titleLevel) const
{
    QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +A->getTitle() +".tex");
    if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
        throw NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);

    wte << "\\chapter{"<< A->getTitle()<<"}" << endl;
    wte << "\\paragraph{Lien vidéo}"<< A->getPath() <<endl;
    wte << "\\paragraph{Description}"<< A->getDesc() << endl;
    description.close();
}

/*!
 * \brief Exporte un fichier Audio en Texte
 * \param A
 * \param titleLevel
 */
void TextExport::exportNote(const Audio* A, unsigned int titleLevel) const
{
    QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +A->getTitle() +".txt");
    if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
        throw NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);

    wte << A->getTitle() <<endl;
    wte << A->getPath() <<endl;
    description.close();
}


/*!
 * \brief Exporte un fichier Video en HTML
 * \param V
 * \param titleLevel
 */
void HTMLExport::exportNote(const Video* V, unsigned int titleLevel) const
{
    QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +V->getTitle() +".html");
    if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
        throw NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);


    wte << "<video controls> " <<endl;
    wte << "\t<source src=\"file:///" << V->getPath() << "\" type=’video/webm; codecs=\"vp8.0, vorbis\"'/> " <<endl;
    wte << "\t<source src=\"file:///"<< V->getPath() <<"\" type=’video/ogg; codecs=\"theora, vorbis\"'/> " <<endl;
    wte << "/t<source src=\"file:///"<< V->getPath() <<"\" type=’video/mp4; codecs=\"avc1.4D401E, mp4a.40.2\"'/> " <<endl;
    wte << "/t<p>" << V->getDesc() << "</p> " <<endl;
    wte << "</video> " <<endl;

    description.close();
}

/*!
 * \brief Exporte un fichier Video en LaTeX
 * \param V
 * \param titleLevel
 */
void TeXExport::exportNote(const Video* V, unsigned int titleLevel) const
{
    QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +V->getTitle() +".tex");
    if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
        throw NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);

    wte << "\\chapter{"<< V->getTitle()<<"}" << endl;
    wte << "\\paragraph{Lien vidéo}"<< V->getPath() <<endl;
    wte << "\\paragraph{Description}"<< V->getDesc() << endl;

    description.close();
}

/*!
 * \brief Exporte un fichier Video en Texte
 * \param V
 * \param titleLevel
 */
void TextExport::exportNote(const Video* V, unsigned int titleLevel) const
{
    QFile description(NotesManager::getInstance()->getWorkSpace() + "/Export/" +V->getTitle() +".txt");
    if (!description.open(QIODevice::WriteOnly | QIODevice::Append))
        throw NotesException("Erreur lors de l'ouverture du fichier");
    QTextStream wte(&description);

    wte << V->getTitle() <<endl;
    wte << V->getPath() <<endl;
    wte << V->getDesc() << endl;
    description.close();
}









