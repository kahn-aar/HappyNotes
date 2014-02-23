#include "tags.h"

TagsManager* TagsManager::instance = 0;

/*!
 * \brief Constructeur
 *
 * Le constructeur de TagsManager créé un TagsManager vide. Il sera rempli à partir du constructeur de NotesManager.
 */
TagsManager::TagsManager()
{

}

TagsManager* TagsManager::getInstance()
{
    if (!instance) instance=new TagsManager;
    return instance;
}

void TagsManager::removeTag(const QString &name)
{
    NotesManager::getInstance()->removeTag(getTag(name));
    tags.removeOne(getTag(name));

}

Tags* TagsManager::getTag(const QString& name)
{
    for(TagsManager::Iterator it = begin(); it != end(); it++)
    {
        if ((*it)->getTagName() == name)
            return (*it);
    }
    return NULL;
}
