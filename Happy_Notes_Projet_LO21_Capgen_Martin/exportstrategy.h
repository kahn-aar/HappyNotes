#ifndef EXPORTSTRATEGY_H
#define EXPORTSTRATEGY_H
#include "note.h"
#include <map>
#include <QMap>
#include <QFile>
#include <iostream>

using namespace std;

class Note;
class Article;
class Document;
class Audio;
class Video;
class Image;


/*!
 * \brief La classe ExportStrategy implémente une interface pour les stratégies d'export
 *
 * Cette classe implémente l'interface pour les exports, en déclarant les méthodes en virtuelles pures. Elle possède aucun attribut, ainsi toutes les méthodes sont constantes.
 * Elle possède de plus une fonction permettant de récupérer un pointeur sur chacune des stratégies.
 * Chacune de ses fonction est redéfinie dans ses classes filles.
 */
class ExportStrategy
{

public:

 virtual void header(const Note* n)const=0;
 virtual void exportNote(Document* doc, unsigned int titelLevel)const=0;
 virtual void exportNote(const Audio* A, unsigned int titleLevel)const=0;
 virtual void exportNote(const Video* V, unsigned int titleLevel)const=0;
 virtual void exportNote(const Image* I, unsigned int titleLevel)const=0;
 virtual void exportNote(const Article *A, unsigned int titleLevel)const=0;
 virtual void exportAsPart(Note* N, unsigned int titleLevel, const QString& filename)const=0;
 virtual void footer(const Note* n)const=0;
 static QMap<QString, ExportStrategy*> getStrategies();
};

/*!
 * \brief La classe HTMLExport implémente la stratégie pour exporter les notes en HTML.
 */
class HTMLExport : public ExportStrategy
{
public:
    void header(const Note* n) const;
    void exportNote(Document* doc, unsigned int titleLevel)const;
    void exportNote(const Audio* A, unsigned int titleLevel) const;
    void exportNote(const Video* V, unsigned int titleLevel) const;
    void exportNote(const Image* I, unsigned int titleLevel) const;
    void exportNote(const Article *A, unsigned int titleLevel) const;
    void exportAsPart(Note* N, unsigned int titleLevel, const QString& filename)const;
    void footer(const Note* n) const;

};

/*!
 * \brief La classe TeXExport implémente la stratégie pour exporter les notes en LaTeX.
 */
class TeXExport : public ExportStrategy
{
public:
    void header(const Note* n) const;
    void exportNote(const Article* A, unsigned int titleLevel) const;
    void exportNote(Document* doc, unsigned int titleLevel) const;
    void exportNote(const Audio* A, unsigned int titleLevel) const;
    void exportNote(const Video* V, unsigned int titleLevel) const;
    void exportNote(const Image* I, unsigned int titleLevel) const;
    void exportAsPart(Note* N, unsigned int titleLevel, const QString& filename) const;
    void footer(const Note* n) const;

};

/*!
 * \brief La classe TextExport implémente la stratégie pour exporter les notes en Texte.
 */
class TextExport : public ExportStrategy
{
public:
    void header(const Note* n) const;
    void exportNote(const Article* A, unsigned int titleLevel) const;
    void exportNote(Document* doc, unsigned int titleLevel) const;
    void exportNote(const Audio* A, unsigned int titleLevel) const;
    void exportNote(const Video* V, unsigned int titleLevel) const;
    void exportNote(const Image* I, unsigned int titleLevel) const;
    void exportAsPart(Note* N, unsigned int titleLevel, const QString& filename) const;
    void footer(const Note* n) const;

};

#endif // EXPORTSTRATEGY_H
