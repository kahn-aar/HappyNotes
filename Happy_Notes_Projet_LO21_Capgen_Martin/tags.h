#ifndef TAGS_H
#define TAGS_H
#include <QString>
#include <QList>
#include "note.h"
#include "notesmanager.h"

/*!
 * \brief La classe Tags implémente les tags.
 * Cette classe implémente les tags d'une façon simple. Chaque tag est identifié par son nom.
 */
class Tags
{
    QString tagname; /**< Il s'agit du nom du tag*/
public:
    /*!
     * \brief Constructeur
     * \param name
     * Le constructeur de cette classe prend un QString comme argument. Cet argument sera le nom du Tag.
     */
    Tags(const QString name) : tagname(name){}
    QString getTagName() const {return tagname;}
    void setTagName(const QString& name){tagname = name;}
};

/*!
 * \brief La classe TagsManager implémente la gestion des Tags.
 *
 * Cette classe possède notament la liste de tous les Tags créés. Cette classe compose les Tags, c'est donc cette classe qui créé les Tags.
 * Il s'agit de plus d'un sigleton, on accède à l'instance gràce à getInstance(), pour créer les Tags par exemple.
 * Il s'agit d'un singleton car il ne peut exister qu'une seule instance de la classe NotesManager
 */
class TagsManager
{
    QList<Tags*> tags; /**< Il s'agit de la liste de tous les tags créés dans l'application*/
    static TagsManager* instance; /**< Il s'agit de l'instance du TagsManager, pour le singleton*/
    TagsManager();
    TagsManager(const TagsManager&);
    TagsManager& operator=(const TagsManager&);

public:
    /*!
     * \brief Récupère l'instance
     * \return
     *
     * Fonction du design pattern Singleton.
     * Si l'instance n'existe pas, cette fonction la créé, sinon elle renvoie un pointeur sur l'instance.
     */
    static TagsManager* getInstance();
    /*!
     * \brief Ajoute un Tags
     * \param tag
     * Cette méthode permet d'ajouter un Tags à la liste tags.
     */
    void addTag(Tags& tag){tags.push_back(&tag);}
    /*!
     * \brief Ajoute un Tags
     * \param name
     * Cette méthode permet d'ajouter un Tags à la liste tags à partir d'un nom. Si il n'existe pas, le Tags sera créé.
     */
    void addTag(const QString& name){Tags* tag = new Tags(name); addTag(*tag);}
    /*!
     * \brief Supprime un Tags
     * \param i
     * Cette méthode supprime un tag
     */
    void removeTag(unsigned int i);
    /*!
     * \brief Supprime un Tags
     * \param i
     * Cette méthode supprime un tag
     */
    void removeTag(const QString& name);
    /*!
     * \brief Vide le TagsManager
     */
    void clear()
    {
        for (Iterator it = begin(); it != end(); it++)
        {
            Tags* tmp = *it;
            delete tmp;
        }
        tags.clear();
    }

    /*!
     * \brief Ajoute un Tags à une Note
     * \param i
     * \param note
     *
     * Cette méthode ajouteun tag à une note
     */
    void setTag(unsigned int i, Note& note);
    /*!
     * \brief Le nombre de Tags
     * \return
     * Cette méthode renvoie le nombre de Tags créés
     */
    int getSize(){return tags.size();}
    /*!
     * \brief Récupération d'un Tags
     * \param name
     * \return
     * Cette méthode renvoye le Tags dont le nom est name.
     */
    Tags* getTag(const QString& name);


    /*!
     * \brief La classe Iterator offre un itérateur sur la liste de Tags.
     *
     * Nous avons utilisé un design pattern Adapteur d'objet sur l'itérateur de QList afin de l'implémenter.
     */
    class Iterator
    {
        QList<Tags*>::iterator itList; /**< Il s'agit du pointeur sur l'élément actuel de la Liste*/
    public:
        Iterator();
        Iterator(QList<Tags*>::iterator iterator) : itList(iterator){}
        Iterator operator++() {++itList; return itList;}
        Iterator operator--() {--itList; return itList;}
        Iterator operator++(int) {return itList++;}
        Iterator operator--(int) {return itList--;}
        Tags* operator*() {return *itList;}
        bool operator==(const Iterator& it) const {return itList == it.itList;}
        bool operator!=(const Iterator& it) const {return itList != it.itList;}
    };
    /*!
     * \brief begin
     * \return
     * Cette méthode renvoie un itérateur sur le premier élément de la liste de Tags.
     */
    Iterator begin(){return Iterator(tags.begin());}
    /*!
     * \brief end
     * \return
     * Cette méthode renvoie un itérateur sur le dernier élément de la liste de Tags.
     */
    Iterator end() {return Iterator(tags.end());}

    /*!
     * \brief La classe Const_Iterator offre un itérateur constant sur la liste de Tags.
     *
     * Nous avons utilisé un design pattern Adapteur d'objet sur l'itérateur de QList afin de l'implémenter.
     */
    class Const_Iterator
    {
        QList<Tags*>::const_iterator itSet;


    public:
        Const_Iterator();
        Const_Iterator(QList<Tags*>::const_iterator iterator) : itSet(iterator){}
        Const_Iterator operator++() {++itSet; return itSet;}
        Const_Iterator operator--() {--itSet; return itSet;}
        Const_Iterator operator++(int) {return itSet++;}
        Const_Iterator operator--(int) {return itSet--;}
        Tags* operator*() const {return *itSet;}
        bool operator==(const Const_Iterator& it) const {return itSet == it.itSet;}
        bool operator!=(const Const_Iterator& it) const {return itSet != it.itSet;}
    };
    Const_Iterator cbegin() const{return Const_Iterator(tags.cbegin());}
    Const_Iterator cend() const{return Const_Iterator(tags.cend());}
};

#endif // TAGS_H
