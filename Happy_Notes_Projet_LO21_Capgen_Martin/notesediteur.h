#ifndef NOTESEDITEUR_H
#define NOTESEDITEUR_H

#include <QMainWindow>
#include <QWidget>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QLabel>
#include <QTabWidget>
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets>
#include <QVBoxLayout>
#include <QString>
#include <QObject>
#include <QMessageBox>
#include <QIcon>
#include <QTreeView>
#include <QStandardItemModel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QRadioButton>
#include "note.h"
#include <iostream>

using namespace std;

/*!
 * \brief La classe NotesEditeur est la fenêtre principale de l'application
 *
 * La fenêtre est composée d'une barre d'outils, du widget central de l'éditeur et du widget de coté avec la liste des notes filtrées.
 */
class NotesEditeur : public QMainWindow
{
    Q_OBJECT

    static NotesEditeur* instance;/**< Il s'agit d'un pointeur l'instance du NotesEditeur*/

    QLineEdit* filter;/**< ligne du filtre*/
    QTabWidget* tab;/**< Onglets*/
    QRadioButton *contient;/**< bouton radio contient*/
    QRadioButton *exact;/**< bouton radio exact*/
    QStandardItemModel* modele;/**< modèle du coté*/
    QListView* view;/**< Vue des notes*/
    QPushButton* corbeille;/**< Bouton de la corbeille*/
    Note* note_actuelle;/**< Pointeur sur la note ouverte*/
    Note* cliked_note;/**< Pointeur sur la note cliquée*/

public:
    explicit NotesEditeur(QWidget *parent = 0);
    static NotesEditeur* getInstance(){ if(!instance) instance = new NotesEditeur; return instance;}
    QStandardItemModel* getModele() const {return modele;}
    void initDockWidgets();
    void initCentralArea();
    void generateModel(QStandardItem* item = 0);
    void refreshModel();
    QString getFilter() const {return filter->text();}
    Note* getNote() const {return note_actuelle;}
    void load_article(Note* note);
    void load_document(Note* note);
    void load_image(Note* note);
    void load_video(Note* note);
    void load_audio(Note* note);
    void enableMenu();
    void disableMenu();

signals:
    
public slots:
    void newArticle();
    void newImage();
    void newVideo();
    void newAudio();
    void newDocument();
    void save();
    void add();
    void addNewTag();
    void addTag();
    void remTag();
    void onSupTag();
    void onTitleChanged(const QString& title );
    void onPathChanged(const QString& path );
    void onDescChanged();
    void onTextChanged();
    void onClick(QModelIndex index);
    void onSimpleClick(QModelIndex index);
    void onFilterModify(QString);
    void onCorbeille();
    void onDelete();
    void onRefresh();
    void onNewWorkspace();
    void onCopyWorkspace();
    void onChangeWorkspace();
    void onExportHTML();
    void onExportLATEX();
    void onExportTXT();
};

/*!
 * \brief La classe SupprTag est une fenêtre permettant de supprimer un Tags.
 *
 * Cette classe créé une fenêtre avec des bouttons radios pour chaque Tags. Le boutton supprime le Tags.
 */
class SupprTag : public QWidget
{
    Q_OBJECT

    QWidget fenetre;/**< Widget conteneur*/
    QGroupBox *groupbox;/**< Contient les boutons radios*/
    QVBoxLayout *vbox; /**< Layout de la fenêtre*/

public:
    SupprTag();
    ~SupprTag();

public slots:
    void onSupprTag();
};

/*!
 * \brief La classe AddTag est une fenêtre permettant d'ajouter un Tags à une Note.
 *
 * Cette classe créé une fenêtre avec des bouttons radios pour chaque Tags. Le boutton applique le Tags choisi à la Note.
 */
class AddTag : public QWidget
{
    Q_OBJECT

    QWidget fenetre;/**< Widget conteneur*/
    QGroupBox *groupbox;/**< Contient les boutons radios*/
    QVBoxLayout *vbox; /**< Layout de la fenêtre*/

public:
    AddTag();

public slots:
    void onAddTag();
};

/*!
 * \brief La classe RemTag est une fenêtre permettant de retirer un Tags à une Note.
 *
 * Cette classe créé une fenêtre avec des bouttons radios pour chaque Tags lié à la note courrante. Le boutton retire le Tags choisi de la Note.
 */
class RemTag : public QWidget
{
    Q_OBJECT

    QWidget fenetre;/**< Widget conteneur*/
    QGroupBox *groupbox;/**< Contient les boutons radios*/
    QVBoxLayout *vbox; /**< Layout de la fenêtre*/

public:
    RemTag();

public slots:
    void onRemTag();
};

/*!
 * \brief La classe Corbeille permet de gérer la Corbeille.
 *
 * Cette classe ouvre une fenêtre où on aura 4 bouttons : supprimer, restaurer, Tout restaurer, Tout supprimer.
 * De plus, elle possède une vue permettant de montrer la liste des Note qui sont dans la Corbeille.
 */
class Corbeille : public QWidget
{
    Q_OBJECT

    QWidget fenetre;/**< Widget conteneur*/
    QVBoxLayout *vbox;/**< layout des boutons*/
    QVBoxLayout *vbox2;/**< layout du modèle*/
    QHBoxLayout *hbox;/**< layout de la fenêtre*/
    QPushButton *restaure;/**< bouton de restauration*/
    QPushButton *_delete;/**< bouton de suppression*/
    QPushButton *restaureAll;/**< bouton pour tout restaurer*/
    QPushButton *deleteAll;/**< bouton pour tout supprimer*/
    QStandardItemModel* modele;/**< Modèle*/
    QListView* view;/**< vue sur la corbeille*/
    Note* note_actuelle; /**< Il s'agit d'un pointeur sur la Note sur laquelle on a cliqué*/

public:
    explicit Corbeille();
    void generateModel(QStandardItem* item = 0);
    void refreshModel();

public slots:
    void onRestaure();
    void onDelete();
    void onRestaureAll();
    void onDeleteAll();
    void onClick(QModelIndex);

};

/*!
 * \brief La classe AddDoc créé une fenêtre permettant de gérer les Document.
 *
 * Cette classe nous permet de gérer les Documents : Ajouter une Note ou supprimer une Note.
 */
class AddDoc : public QWidget
{
    Q_OBJECT

    QWidget fenetre; /**< Widget de la fenêtre*/
    QVBoxLayout *vbox;/**< layout de gauche*/
    QVBoxLayout *vbox2;/**< layout de droite*/
    QHBoxLayout *hbox;/**< layout de la fenêtre*/
    QStandardItemModel* modele_gauche;/**< modèle de gauche*/
    QListView* view_gauche;/**< Vue des notes qui ne sont pas dans le document*/
    QStandardItemModel* modele_droite;/**< modèle de gauche*/
    QListView* view_droite;/**< Vue des notes qui sont dans le document*/
    Note* note_actuelle_gauche;/**< Pointeur sur la note cliquée de gauche*/
    Note* note_actuelle_droite;/**< Pointeur sur la note cliquée de droite*/
    QPushButton* ajouter;/**< Bouton d'ajout*/
    QPushButton* supprimer;/**< Bouton de suppression*/
    Document* doc;/**< Pointeur sur le document que l'on modifie */
public:
    AddDoc(Document*docu);
    void generateModel_gauche(QStandardItem* item = 0);
    void generateModel_droit(QStandardItem* item = 0);
    void refreshModel();
public slots:
    void onAddDoc();
    void onClickGauche(QModelIndex index);
    void onClickDroit(QModelIndex index);
    void onRemDoc();

};

#endif // NOTESEDITEUR_H
