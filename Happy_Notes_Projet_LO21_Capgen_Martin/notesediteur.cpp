#include "notesediteur.h"

NotesEditeur* NotesEditeur::instance = 0;

/*!
 * \brief Constructeur
 * \param parent
 *
 * Le constructeur de NotesEditeur va créer le menu de la fenêtre principale, en initialisant les signaux, les slots, les icones et les raccourcis. De plus on initialise la barre d'outils, puis on appelle les méthodes initDockWidget() et initCentralWidget() pour créer les widgets de la fenêtre.
 */
NotesEditeur::NotesEditeur(QWidget *parent) :
    QMainWindow(parent), note_actuelle(0), cliked_note(0)
{
    setWindowTitle(QString("Happy Notes - The editor"));
    setWindowIcon(QIcon("../icons/panda.png"));
    QMenu* mfichier = menuBar()->addMenu("&Fichier");
    QMenu* mEdition = menuBar()->addMenu("&Edition");
    QMenu* mNouveau = mfichier->addMenu("&Nouveau");
    QMenu* mDocument = menuBar()->addMenu("&Document");
    QMenu* mTags = menuBar()->addMenu("&Tags");
    QMenu* mExport = menuBar() -> addMenu("&Export");
    QMenu* mCorbeille = menuBar()->addMenu("&Corbeille");

    QAction* actionNewArticle = mNouveau->addAction("Article");
    QAction* actionNewImage = mNouveau->addAction("Image");
    QAction* actionNewAudio = mNouveau->addAction("Enregistrement Audio");
    QAction* actionNewVideo = mNouveau->addAction("Enregistrement Video");
    QAction* actionNewDocument = mNouveau->addAction("Document");
    mNouveau->addSeparator();
    QMenu* menuChangeDir = mfichier->addMenu("Changer de Workspace");
    QAction* actionNewWorkspace = menuChangeDir->addAction(QIcon("../icons/dossier.png"),"Nouveau workspace");
    QAction* actionCopyWorkspace = menuChangeDir->addAction(QIcon("../icons/dossier.png"),"Déplacer workspace");
    QAction* actionChangeWorkspace = menuChangeDir->addAction(QIcon("../icons/dossier.png"),"Changer de workspace");
    mfichier->addSeparator();
    QAction* actionEnregistrer = mfichier->addAction("Enregistrer");
    QAction* actionAdd = mDocument->addAction("Ajouter au document");

    QAction* actionExportHTML = mExport->addAction("Exporter en HTML");
    QAction* actionExportLATEX = mExport->addAction("Exporter en LaTex");
    QAction* actionExportTXT = mExport->addAction("Exporter en texte");

    mfichier->addSeparator();
    QAction* actionQuitter = mfichier->addAction("&Quitter");
    QAction* actionEdition = mEdition->addAction("Edit");
    QAction* actionNewTag = mTags->addAction(QIcon("../icons/tag.png"),"Nouveau tag");
    QAction* actionSupTag = mTags->addAction(QIcon("../icons/tag.png"),"Supprimer Tag");
    QAction* actionAddTag = mTags->addAction("Ajouter tag");
    QAction* actionRemTag = mTags->addAction("Retirer tag");
    QAction* actionCorbeille = mCorbeille->addAction(QIcon("../icons/corbeille2.png"),"Corbeille");
    QAction* actionDelete = mEdition->addAction("Supprimer note");
    QAction* actionRefresh = mEdition->addAction(QIcon("../icons/refresh.png"), "rafraichir");


    //Connection
    QObject::connect(actionNewArticle, SIGNAL(triggered()), this, SLOT(newArticle()));
    QObject::connect(actionNewDocument, SIGNAL(triggered()), this, SLOT(newDocument()));
    QObject::connect(actionNewImage, SIGNAL(triggered()), this, SLOT(newImage()));
    QObject::connect(actionNewAudio, SIGNAL(triggered()), this, SLOT(newAudio()));
    QObject::connect(actionNewVideo, SIGNAL(triggered()), this, SLOT(newVideo()));
    QObject::connect(actionEnregistrer, SIGNAL(triggered()), this, SLOT(save()));
    QObject::connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
    QObject::connect(actionAdd, SIGNAL(triggered()), this, SLOT(add()));
    QObject::connect(actionNewTag, SIGNAL(triggered()), this, SLOT(addNewTag()));
    QObject::connect(actionAddTag, SIGNAL(triggered()), this, SLOT(addTag()));
    QObject::connect(actionRemTag, SIGNAL(triggered()), this, SLOT(remTag()));
    QObject::connect(actionCorbeille, SIGNAL(triggered()), this, SLOT(onCorbeille()));
    QObject::connect(actionDelete, SIGNAL(triggered()), this, SLOT(onDelete()));
    QObject::connect(actionRefresh, SIGNAL(triggered()), this, SLOT(onRefresh()));
    QObject::connect(actionSupTag, SIGNAL(triggered()), this, SLOT(onSupTag()));
    QObject::connect(actionNewWorkspace, SIGNAL(triggered()), this, SLOT(onNewWorkspace()));
    QObject::connect(actionCopyWorkspace, SIGNAL(triggered()), this, SLOT(onCopyWorkspace()));
    QObject::connect(actionChangeWorkspace, SIGNAL(triggered()), this, SLOT(onChangeWorkspace()));
    QObject::connect(actionExportHTML, SIGNAL(triggered()), this, SLOT(onExportHTML()));
    QObject::connect(actionExportLATEX, SIGNAL(triggered()), this, SLOT(onExportLATEX()));
    QObject::connect(actionExportTXT, SIGNAL(triggered()), this, SLOT(onExportTXT()));


    actionQuitter->setIcon(QIcon("../icons/quitter.png"));
    actionNewArticle->setIcon(QIcon("../icons/note.png"));
    actionNewDocument->setIcon(QIcon("../icons/dossier.png"));
    actionNewImage->setIcon(QIcon("../icons/photo.png"));
    actionNewVideo->setIcon(QIcon("../icons/movie.png"));
    actionNewAudio->setIcon(QIcon("../icons/audio.png"));
    actionEnregistrer->setIcon(QIcon("../icons/save.png"));
    actionAddTag->setIcon(QIcon("../icons/plus2.png"));
    actionRemTag->setIcon(QIcon("../icons/moins.png"));
    actionDelete->setIcon(QIcon("../icons/croix.png"));
    actionExportHTML->setIcon(QIcon("../icons/export.png"));
    actionExportLATEX->setIcon(QIcon("../icons/export.png"));
    actionExportTXT->setIcon(QIcon("../icons/export.png"));

    actionAddTag->setDisabled(true);
    actionRemTag->setDisabled(true);
    actionAdd->setDisabled(true);
    actionDelete->setDisabled(true);
    actionExportHTML->setDisabled(true);
    actionExportLATEX->setDisabled(true);
    actionExportTXT->setDisabled(true);
    actionDelete->setShortcut(tr("Del"));
    actionEnregistrer->setShortcut(tr("Ctrl+S"));
    actionNewArticle->setShortcut(tr("Ctrl+N"));

    QToolBar* toolBarOpen = addToolBar("open");
    toolBarOpen->addAction(actionNewArticle);
    toolBarOpen->addAction(actionNewAudio);
    toolBarOpen->addAction(actionNewVideo);
    toolBarOpen->addAction(actionNewImage);
    toolBarOpen->addAction(actionNewDocument);
    toolBarOpen->addSeparator();
    toolBarOpen->addAction(actionRefresh);
    toolBarOpen->addAction(actionEnregistrer);
    toolBarOpen->addSeparator();
    toolBarOpen->addAction(actionDelete);
    toolBarOpen->addAction(actionCorbeille);
   // toolBarOpen->addAction(actionExport);


    initCentralArea();
    initDockWidgets();
}

/*!
 * \brief Créée le widget central : les onglets.
 */
void NotesEditeur::initCentralArea()
{
    QWidget *centralZone = new QWidget;
    QHBoxLayout *mainLayout = new QHBoxLayout;

    tab = new QTabWidget;
    //tab->setTabsClosable(false);
    mainLayout->addWidget(tab);
    centralZone->setLayout(mainLayout);
    setCentralWidget(centralZone);

    //connect( tab, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabClosed(int)));
}

/*!
 * \brief Créée les widget sur la gauche : Le filtre, les boutons radios, le modèle ainsi que le bouton corbeille.
 */
void NotesEditeur::initDockWidgets()
{
    QVBoxLayout *leftLayout = new QVBoxLayout;
    QVBoxLayout *dirLayout = new QVBoxLayout;
    QWidget *mainDockWidget = new QWidget;

    filter = new QLineEdit;
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("Tag filter :  ", filter);
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    contient = new QRadioButton("Contient");
    exact = new QRadioButton("Exact");
    buttonLayout->addWidget(contient);
    buttonLayout->addWidget(exact);
    contient->setChecked(true);
    modele = new QStandardItemModel;
    view = new QListView;
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setModel(modele);
    dirLayout->addWidget(view);
    generateModel();
    corbeille = new QPushButton(QIcon("../icons/corbeille2.png"),"Corbeille");
    dirLayout->addWidget(corbeille);
    leftLayout->addLayout(formLayout);
    leftLayout->addLayout(buttonLayout);
    leftLayout->addLayout(dirLayout);

    mainDockWidget->setLayout(leftLayout);
    QDockWidget *dock = new QDockWidget;//("  Note explorer");
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dock->setWidget(mainDockWidget);
    mainDockWidget->setMinimumWidth(200);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    connect(view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onClick(QModelIndex)));
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(onSimpleClick(QModelIndex)));
    connect(filter, SIGNAL(textChanged(QString)), this, SLOT(onFilterModify(QString)));
    connect(corbeille, SIGNAL(clicked()), this, SLOT(onCorbeille()));
}

/*!
 * \brief Génère un modèle
 * \param item
 *
 * Cette méthode génère un modèle à partir de la liste filtrée du NotesManager.
 */
void NotesEditeur::generateModel( QStandardItem* item)
{
    QStandardItem* itembis;
    if( !item )
        for( NotesManager::Iterator it = NotesManager::getInstance()->begin_filt(); it != NotesManager::getInstance()->end_filt(); it++ )
        {
            if( (*it)->getTitle() != "" ){
                QString type = NotesManager::getInstance()->getType(*(*it));
                if(type == "Article")
                {
                    itembis = new QStandardItem( QIcon("../icons/note.png"), (*it)->getTitle() );
                    modele->appendRow( itembis );
                }
                else if(type == "Document")
                {
                    itembis = new QStandardItem( QIcon("../icons/dossier.png"), (*it)->getTitle() );
                    modele->appendRow( itembis );
                }
                else if(type == "Video")
                {
                    itembis = new QStandardItem( QIcon("../icons/movie.png"), (*it)->getTitle() );
                    modele->appendRow( itembis );
                }
                else if(type == "Audio")
                {
                    itembis = new QStandardItem( QIcon("../icons/audio.png"), (*it)->getTitle() );
                    modele->appendRow( itembis );
                }
                else if(type == "Image")
                {
                    itembis = new QStandardItem( QIcon("../icons/photo.png"), (*it)->getTitle() );
                    modele->appendRow( itembis );
                }

            }
        }

    view->setModel(modele);
}

/*!
 * \brief Rafraichit le modèle
 *
 * Cette méthode permet de rafraichir les modèles. Elle va vider le modèle courrant et générer un nouveau modèle.
 */
void NotesEditeur::refreshModel()
{
    modele->clear();
    generateModel();
}

/*!
 * \brief Slot lors du double clic sur la vue
 * \param index : index de la note cliquée
 *
 * Ce slot ouvre la Note dans le widget central. Pour cela on entre un pointeur sur cette note dans note_actuelle, on retire la note précédente de l'écran et on ajoute la nouvelle (avec setOnScreen()).
 * Ce slot appelle la méthode load correspondant au type de le Note pour la charger dans le widget central.
 */
void NotesEditeur::onClick(QModelIndex index)
{
   QVariant note = modele->data(index, Qt::DisplayRole);
   QString nom = note.toString();
   Note* note2 = NotesManager::getInstance()->getNote(nom);
   if(!note2->getScreen())
   {
       QString type = NotesManager::getInstance()->getType(*note2);
       if(type == "Article") load_article(note2);
       else if(type == "Document") load_document(note2);
       else if(type == "Video") load_video(note2);
       else if(type == "Audio") load_audio(note2);
       else if(type == "Image") load_image(note2);
       if (note_actuelle)note_actuelle->removeFromScreen();
       note_actuelle = note2;
       note_actuelle->setOnScreen();
       enableMenu();
   }
}

/*!
 * \brief Réactive les menus
 */
void NotesEditeur::enableMenu()
{
    cout << "enablemenu" << endl;
    foreach (QAction* action, NotesEditeur::getInstance()->menuBar()->actions())
    {
        QMenu* menu = action->menu();
        foreach (QAction* action2, menu->actions())
        {
            if (NotesManager::getInstance()->getType(*note_actuelle) == "Document" && action2->text() == "Ajouter au document")
                action2->setEnabled(true);
            else
                if (action2->text() == "Ajouter au document")
                    action2->setDisabled(true);
        if((action2->text() == "Ajouter tag") || (action2->text() == "Retirer tag") || (action2->text() == "Supprimer note") || (action2->text() == "Exporter en HTML") || (action2->text() == "Exporter en LaTex") || (action2->text() == "Exporter en texte"))
           action2->setEnabled(true);
    }}
}

/*!
 * \brief Désactive les menus
 */
void NotesEditeur::disableMenu()
{
    cout << "disablemenu" << endl;
    foreach (QAction* action, NotesEditeur::getInstance()->menuBar()->actions())
    {
        QMenu* menu = action->menu();
        foreach (QAction* action2, menu->actions())
        {
        if((action2->text() == "Ajouter tag") || (action2->text() == "Retirer tag") || (action2->text() == "Ajouter au document") || (action2->text() == "Supprimer note") || (action2->text() == "Exporter en HTML") || (action2->text() == "Exporter en LaTex") || (action2->text() == "Exporter en texte"))
           action2->setDisabled(true);
    }}
}

/*!
 * \brief Slot activé lors d'un simple clic sur la vue
 * \param index : index de la note cliquée
 *
 * Charge un pointeur sur la note sélectionnée dans clicked_note et active les menus correspondants.
 */
void NotesEditeur::onSimpleClick(QModelIndex index)
{
    QVariant note = modele->data(index, Qt::DisplayRole);
    QString nom = note.toString();
    cliked_note = NotesManager::getInstance()->getNote(nom);
    foreach (QAction* action, NotesEditeur::getInstance()->menuBar()->actions())
    {
        QMenu* menu = action->menu();
        foreach (QAction* action2, menu->actions())
        {
            if(action2->text() == "Supprimer note" || action2->text()=="Exporter en HTML" || action2->text()=="Exporter en LaTex" || action2->text()=="Exporter en texte")
               action2->setEnabled(true);
        }
    }
}

/*!
 * \brief Slot activé lors de la modification du texte du filtre
 * \param filter : Texte contenu dans le filtre
 *
 * Ce slot va appeler les méthodes de NotesManager pour filtrer la liste de Notes selon le bouton enclanché.
 */
void NotesEditeur::onFilterModify(QString filter)
{
    if (exact->isChecked())
    {
        if (filter == "")
        {
            NotesManager::getInstance()->deleteFiltered();
            NotesManager::getInstance()->initializeFiltered();
        }
        else
        {
            NotesManager::getInstance()->filter(filter);
        }
    }
    else
    {
        if (filter == "")
        {
            NotesManager::getInstance()->deleteFiltered();
            NotesManager::getInstance()->initializeFiltered();
        }
        else
        {
            NotesManager::getInstance()->filter_content(filter);
        }
    }
    refreshModel();
}

/*!
 * \brief Slot qui enregistre les modifications faites sur le titre des Notes
 * \param title
 *
 * La modification du titre entraine la modification du titre de l'onglet. Une modification ajoute de plus une étoile après le titre de l'onglet pour la montrer.
 */
void NotesEditeur::onTitleChanged(const QString& title )
{
    Note* note = getNote();
    note->setTitle(title);
    note->setModified();
    QString titre = title + "*";
    tab->setTabText(tab->currentIndex(), titre);
}

/*!
 * \brief Slot qui enregistre les modifications faites sur le chemin des Medias.
 * \param path
 *
 * Une modification ajoute une étoile après le titre de l'onglet pour le montrer.
 */
void NotesEditeur::onPathChanged(const QString& path )
{
    Note* note = getNote();
    Media* med = static_cast< Media* >(note);
    med->setPath(path);
    note->setModified();
    QString titre = note->getTitle() + "*";
    tab->setTabText(tab->currentIndex(), titre);
}

/*!
 * \brief Slot qui enregistre les modifications faites sur la description des Médias.
 *
 * Une modification ajoute une étoile après le titre de l'onglet pour le montrer.
 */
void NotesEditeur::onDescChanged()
{
    Note* note = getNote();
    Media* med = static_cast< Media* >(note);
    QTextEdit* wid = tab->currentWidget()->findChild<QTextEdit*>();
    med->setDesc(wid->toPlainText()); med->setModified();
    QString titre = note->getTitle() + "*";
    tab->setTabText(tab->currentIndex(), titre);
}

/*!
 * \brief Slot qui enregistre les modifications faites sur le texte des Articles
 *
 * Une modification ajoute une étoile après le titre de l'onglet pour le montrer.
 */
void NotesEditeur::onTextChanged()
{
    Note* note = getNote();
    Article* art = static_cast< Article* >(note);
    QTextEdit* wid = tab->currentWidget()->findChild<QTextEdit*>();
    art->setText(wid->toPlainText());
    art->setModified();QString titre = note->getTitle() + "*";
    tab->setTabText(tab->currentIndex(), titre);
}

/*!
 * \brief Slot ouvrant la fenêtre de la corbeille.
 *
 * Appelle le constructeur de la classe Corbeille.
 */
void NotesEditeur::onCorbeille()
{
    Corbeille* c = new Corbeille() ;
}

/*!
 * \brief Slot de la suppression des Notes
 *
 * Il possède un raccourci avec le bouton DEL du clavier, quand on à cliqué sur une note. Ainsi cette fonction va envoyer à la corbeille en priorité la clicked_note.
 */
void NotesEditeur::onDelete()
{
    if(cliked_note && note_actuelle)
    {
        NotesManager::getInstance()->trash(cliked_note);
        if(cliked_note == note_actuelle)
        {
            tab->clear();
            note_actuelle->removeFromScreen();
            note_actuelle = 0;
            disableMenu();

        }
        cliked_note = 0;
    }
    else if (cliked_note)
    {
        NotesManager::getInstance()->trash(cliked_note);
        cliked_note = 0;
        foreach (QAction* action, NotesEditeur::getInstance()->menuBar()->actions())
        {
            QMenu* menu = action->menu();
            foreach (QAction* action2, menu->actions())
            {
                if(action2->text() == "Supprimer note")
                   action2->setDisabled(true);
            }
        }
    }
    else if (note_actuelle)
    {
        NotesManager::getInstance()->trash(note_actuelle);
        tab->clear();
        note_actuelle->removeFromScreen();
        note_actuelle = 0;
        disableMenu();
    }

    refreshModel();
}




/*!
 * \brief Exporte la note_actuelle en HTML
 */
void NotesEditeur::onExportHTML()
{
    try
    {
        NotesManager::getInstance()->exporterNote("HTML", note_actuelle);
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("La note a bien été exportée en HTML !");
        msgBox.setWindowIcon(QIcon("../icons/panda.png"));
        msgBox.setWindowTitle("Export reussi !");
        msgBox.exec();
    }
    catch (NotesException ne)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("La note n'a pas été exportée en HTML !");
        msgBox.setWindowIcon(QIcon("../icons/panda.png"));
        msgBox.setWindowTitle("Export raté !");
        msgBox.exec();
    }
}

/*!
 * \brief Exporte la note_actuelle en LaTeX
 */
void NotesEditeur::onExportLATEX()
{
    try
    {
        NotesManager::getInstance()->exporterNote("TeX", note_actuelle);
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("La note a bien été exportée en LaTeX !");
        msgBox.setWindowIcon(QIcon("../icons/panda.png"));
        msgBox.setWindowTitle("Export reussi !");
        msgBox.exec();
    }
    catch (NotesException ne)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("La note n'a pas été exportée en LaTeX !");
        msgBox.setWindowIcon(QIcon("../icons/panda.png"));
        msgBox.setWindowTitle("Export raté !");
        msgBox.exec();
    }
}

/*!
 * \brief Exporte la note_actuelle en texte
 */
void NotesEditeur::onExportTXT()
{
    try
    {
        NotesManager::getInstance()->exporterNote("Texte", note_actuelle);
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("La note a bien été exportée en texte !");
        msgBox.setWindowIcon(QIcon("../icons/panda.png"));
        msgBox.setWindowTitle("Export reussi !");
        msgBox.exec();
    }
    catch (NotesException ne)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("La note n'a pas été exportée en LaTeX !");
        msgBox.setWindowIcon(QIcon("../icons/panda.png"));
        msgBox.setWindowTitle("Export raté !");
        msgBox.exec();
    }

}

/*!
 * \brief Rafraichit la vue
 */
void NotesEditeur::onRefresh()
{
    refreshModel();
}


/*!
 * \brief Ouvre la fenêtre de suppression de Tags.
 */
void NotesEditeur::onSupTag()
{
    SupprTag* t = new SupprTag();

}

/*!
 * \brief Ouvre une fenêtre pour choisir son nouveau workspace (Tout nouveau).
 */
void NotesEditeur::onNewWorkspace()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Nouveau WorkSpace"),
                                                     NotesManager::getInstance()->getWorkSpace(),
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    if (dir != "")
        NotesManager::getInstance()->setWorkspace(dir);
    tab->clear();
}

/*!
 * \brief Ouvre une fenêtre pour choisir l'emplacement de copie du workspace.
 */
void NotesEditeur::onCopyWorkspace()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Copie du WorkSpace"),
                                                     NotesManager::getInstance()->getWorkSpace(),
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    if (dir != "")
        NotesManager::getInstance()->setCopyWorkspace(dir);
    tab->clear();
}

/*!
 * \brief Ouvre une fenêtre pour changer de workspace
 */
void NotesEditeur::onChangeWorkspace()
{
    QString desc = QFileDialog::getOpenFileName(this, tr("Change workspace"), NotesManager::getInstance()->getWorkSpace(), tr("Description file (*.desc)"));
    if (desc != "")
        NotesManager::getInstance()->setChangeWorkspace(desc);
    tab->clear();
}

/*!
 * \brief Méthode de création d'un nouvel article
 *
 * Cette méthode appelle le NotesManager pour créer un nouvel Article. Il créé de plus des widgets pour modifier l'article.
 */
void NotesEditeur::newArticle()
{
    Note* art = NotesManager::getInstance()->getNewNote("Article" );
    if (note_actuelle)
        note_actuelle->removeFromScreen();
    note_actuelle = art;
    QWidget* mainWidTab = new QWidget;
    QVBoxLayout* mainLayTab = new QVBoxLayout;
    QLineEdit* mainLineTab = new QLineEdit;
    QTextEdit* mainTextTab = new QTextEdit;
    mainLayTab->addWidget(mainLineTab);
    mainLayTab->addWidget(mainTextTab);
    mainWidTab->setLayout(mainLayTab);

    tab->clear();
    tab->addTab(mainWidTab, "nouvel article" );
    tab->setCurrentWidget(mainWidTab);

    art->setOnScreen();
    refreshModel();
    enableMenu();
    connect( mainLineTab, SIGNAL(textChanged(const QString&)), this, SLOT(onTitleChanged(const QString&)) );
    connect( mainTextTab, SIGNAL(textChanged()), this, SLOT(onTextChanged()) );
}


/*!
 * \brief Créé une nouvelle note de type image.
 *
 * Ouvre une fenêtre permettant de charger une image existante. Du chemin de l'image, on en extrait le nom du fichier que l'on met en tant que titre et on insère le chemin dans la note. On ouvre aussi un widget permettant la modification de la Note.
 * De plus, cette méthode ouvre un nouvel onglet où l'on peut voir un apperçu de l'image.
 */
void NotesEditeur::newImage()
{
    if (note_actuelle)
        note_actuelle->removeFromScreen();
    Note* img = NotesManager::getInstance()->getNewNote("Image" );
    note_actuelle = img;
    QString chemin = QFileDialog::getOpenFileName(this, "Ouvrir une image", NotesManager::getInstance()->getWorkSpace(), tr("Image Files (*.png *.jpg *.bmp)"));
    if (chemin != "")
    {
        QStringList name = chemin.split("/");
        QString un_name = name.last();
        name = un_name.split(".");
        un_name = name.first();
        QWidget* mainWidTab = new QWidget;
        QVBoxLayout* mainLayTab = new QVBoxLayout;
        QLineEdit* lineTitre = new QLineEdit(un_name);
        QLineEdit* linePath = new QLineEdit(chemin);
        QTextEdit* textDesc = new QTextEdit;
        mainLayTab->addWidget(lineTitre);
        mainLayTab->addWidget(linePath);
        mainLayTab->addWidget(textDesc);
        mainWidTab->setLayout(mainLayTab);

        tab->clear();

        tab->addTab(mainWidTab, un_name );
        QLabel *imageLabel = new QLabel;
        QImage image(chemin);
        imageLabel->setPixmap(QPixmap::fromImage(image));

        QScrollArea *scrollArea = new QScrollArea;
        scrollArea->setBackgroundRole(QPalette::Dark);
        scrollArea->setWidget(imageLabel);
        tab->addTab(imageLabel, "Image");
        tab->setCurrentWidget(mainWidTab);
        img->setTitle(un_name);
        img->setModified();

        Media* med = dynamic_cast<Media*>(img);
        med->setPath(chemin);
        note_actuelle->setOnScreen();
        refreshModel();
        enableMenu();
        connect( lineTitre, SIGNAL(textChanged(const QString&)), this, SLOT(onTitleChanged(const QString&)) );
        connect( linePath, SIGNAL(textChanged(const QString&)), this, SLOT(onPathChanged(const QString&)) );
        connect( textDesc, SIGNAL(textChanged()), this, SLOT(onDescChanged()) );

    }
}

/*!
 * \brief Créé une nouvelle note de type audio.
 *
 * Ouvre une fenêtre permettant de charger un fichier audio existante. Du chemin du fichier, on en extrait le nom du fichier que l'on met en tant que titre et on insère le chemin dans la note. On ouvre aussi un widget permettant la modification de la Note.
 */
void NotesEditeur::newAudio()
{
    if (note_actuelle)
        note_actuelle->removeFromScreen();
    Note* aud = NotesManager::getInstance()->getNewNote("Audio" );
    note_actuelle = aud;
    QString chemin = QFileDialog::getOpenFileName(this, "Ouvrir un fichier audio", NotesManager::getInstance()->getWorkSpace(), tr("Audio Files (*.wav *.mp3)"));
    if (chemin != "")
    {
        QStringList name = chemin.split("/");
        QString un_name = name.last();
        name = un_name.split(".");
        un_name = name.first();
        QWidget* mainWidTab = new QWidget;
        QVBoxLayout* mainLayTab = new QVBoxLayout;
        QLineEdit* lineTitre = new QLineEdit(un_name);
        QLineEdit* linePath = new QLineEdit(chemin);
        QTextEdit* textDesc = new QTextEdit;
        mainLayTab->addWidget(lineTitre);
        mainLayTab->addWidget(linePath);
        mainLayTab->addWidget(textDesc);
        mainWidTab->setLayout(mainLayTab);

        tab->clear();
        tab->addTab(mainWidTab, un_name );
        tab->setCurrentWidget(mainWidTab);
        aud->setTitle(un_name);
        Media* med = dynamic_cast<Media*>(aud);
        med->setPath(chemin);
        note_actuelle->setOnScreen();
        refreshModel();
        enableMenu();
        med->setModified();
        connect( lineTitre, SIGNAL(textChanged(const QString&)), this, SLOT(onTitleChanged(const QString&)) );
        connect( linePath, SIGNAL(textChanged(const QString&)), this, SLOT(onPathChanged(const QString&)) );
        connect( textDesc, SIGNAL(textChanged()), this, SLOT(onDescChanged()) );

    }
}

/*!
 * \brief Créé une nouvelle note de type video.
 *
 * Ouvre une fenêtre permettant de charger un fichier video existante. Du chemin du fichier, on en extrait le nom du fichier que l'on met en tant que titre et on insère le chemin dans la note. On ouvre aussi un widget permettant la modification de la Note.
 */
void NotesEditeur::newVideo()
{
    if (note_actuelle)
        note_actuelle->removeFromScreen();
    Note* vid = NotesManager::getInstance()->getNewNote("Video" );
    note_actuelle = vid;
    QString chemin = QFileDialog::getOpenFileName(this, "Ouvrir une video", NotesManager::getInstance()->getWorkSpace(), tr("Video Files (*.avi *.mp4 *.wmv)"));
    if (chemin != "")
    {
        QStringList name = chemin.split("/");
        QString un_name = name.last();
        name = un_name.split(".");
        un_name = name.first();
        QWidget* mainWidTab = new QWidget;
        QVBoxLayout* mainLayTab = new QVBoxLayout;
        QLineEdit* lineTitre = new QLineEdit(un_name);
        QLineEdit* linePath = new QLineEdit(chemin);
        QTextEdit* textDesc = new QTextEdit;
        mainLayTab->addWidget(lineTitre);
        mainLayTab->addWidget(linePath);
        mainLayTab->addWidget(textDesc);
        mainWidTab->setLayout(mainLayTab);

        tab->clear();
        tab->addTab(mainWidTab, un_name );
        tab->setCurrentWidget(mainWidTab);
        vid->setTitle(un_name);
        Media* med = dynamic_cast<Media*>(vid);
        med->setPath(chemin);
        note_actuelle->setOnScreen();
        refreshModel();
        enableMenu();
        med->setModified();
        connect( lineTitre, SIGNAL(textChanged(const QString&)), this, SLOT(onTitleChanged(const QString&)) );
        connect( linePath, SIGNAL(textChanged(const QString&)), this, SLOT(onPathChanged(const QString&)) );
        connect( textDesc, SIGNAL(textChanged()), this, SLOT(onDescChanged()) );

    }
}

/*!
 * \brief Créé une nouvelle note de type Document.
 *
 * Cette méthode ouvre le widget avec uniquement un titre.
 */
void NotesEditeur::newDocument()
{
    if (note_actuelle)
        note_actuelle->removeFromScreen();
    Note* doc = NotesManager::getInstance()->getNewNote("Document");
    note_actuelle = doc;
    QWidget* mainWidTab = new QWidget;
    QVBoxLayout* mainLayTab = new QVBoxLayout;
    QLineEdit* mainLineTab = new QLineEdit;
    mainLayTab->addWidget(mainLineTab);
    mainWidTab->setLayout(mainLayTab);

    tab->clear();
    tab->addTab(mainWidTab, "nouveau document" );
    tab->setCurrentWidget(mainWidTab);
    note_actuelle = doc;
    note_actuelle->setOnScreen();
    refreshModel();
    enableMenu();
    connect( mainLineTab, SIGNAL(textChanged(const QString&)), this, SLOT(onTitleChanged(const QString&)) );

}

/*!
 * \brief Cette méthode charge un Article dans l'éditeur
 * \param note
 * Si la note n'a pas été chargée (si elle ne possède pas encore son texte), alors elle est chargée à travers sa méthode load. L'éditeur est ensuite chargé avec ses caractéristiques.
 */
void NotesEditeur::load_article(Note *note)
{
    if (note_actuelle)
        note_actuelle->removeFromScreen();
    cout << "entrée dans load" << endl;
    Article* a = static_cast<Article*>(note);
    if( !note->getLoaded()) {
        cout << "pas chargée"<<endl;
        a->load();
        a->setLoaded();
        cout << "chargée" << endl;
    }
    QWidget* mainWidTab = new QWidget;
    QVBoxLayout* mainLayTab = new QVBoxLayout;
    QLineEdit* mainLineTab = new QLineEdit;
    mainLineTab->setText(a->getTitle());
    QTextEdit* mainTextTab = new QTextEdit;
    mainTextTab->setText(a->getText());
    mainLayTab->addWidget(mainLineTab);
    mainLayTab->addWidget(mainTextTab);
    mainWidTab->setLayout(mainLayTab);
    tab->clear();
    tab->addTab(mainWidTab, a->getTitle() );
    tab->setCurrentWidget(mainWidTab);
    /*QWidget* lapin = new QWidget;
    QVBoxLayout*tab1 = new QVBoxLayout;
    lapin->setLayout(tab1);
    tab->addTab(lapin, "HTML");
    QWidget* lapin2 = new QWidget;
    QVBoxLayout*tab2 = new QVBoxLayout;
    lapin2->setLayout(tab2);
    tab->addTab(lapin2, "TeX");*/
    note_actuelle = a;
    note_actuelle->setOnScreen();
    connect( mainLineTab, SIGNAL(textChanged(const QString&)), this, SLOT(onTitleChanged(const QString&)) );
        connect( mainTextTab, SIGNAL(textChanged()), this, SLOT(onTextChanged()) );
}

/*!
 * \brief Cette méthode charge un document dans l'éditeur
 * \param note
 * Si la note n'a pas été chargée (si elle ne possède pas encore ses Notes), alors elle est chargée à travers sa méthode load. L'éditeur est ensuite chargé avec ses caractéristiques. Seul le titre du document est modifiable. Les Notes insérée sont insérées dans une ScrollArea, ainsi toutes les notes peuvent être visibles.
 */
void NotesEditeur::load_document(Note* note)
{
    if (note_actuelle)
        note_actuelle->removeFromScreen();
    Document* doc = static_cast<Document*>(note);
    if( !note->getLoaded()) {
        cout << "pas chargée"<<endl;
        doc->load();
        doc->setLoaded();
        cout << "chargée" << endl;
    }

    QWidget* mainWidTab = new QWidget;
    QVBoxLayout* mainLayTab = new QVBoxLayout;

    QLineEdit* mainLineTab = new QLineEdit;
    mainLineTab->setText(doc->getTitle());
    mainLayTab->addWidget(mainLineTab);
    cout << "itérator hééh" << endl;
    if (doc->getNb())
    {
        for (Document::Iterator it = doc->begin(); it != doc->end(); it++)
        {
            if(NotesManager::getInstance()->isInBin(*it))
                doc->removeNote(*it);
            else
                {
                cout << "non mais ya pas d'article..." << endl;
                QLineEdit* noteTitle = new QLineEdit;
                noteTitle->setText((*it)->getTitle());
                mainLayTab->addWidget(noteTitle);
                QString type = NotesManager::getInstance()->getType(*(*it));
                if (!(*it)->getLoaded())
                {
                    (*it)->load();
                    (*it)->setLoaded();
                }
                if(type == "Article")
                {
                    Article* art = dynamic_cast<Article*>(*it);
                    QTextEdit* noteText = new QTextEdit;
                    noteText->setText(art->getText());
                    mainLayTab->addWidget(noteText);
                }
                if (type == "Audio" || type == "Video" || type == "Image")
                {
                    Media* med = dynamic_cast<Media*>(*it);
                    QLineEdit* url = new QLineEdit;
                    url->setText(med->getPath());
                    QTextEdit* noteText = new QTextEdit;
                    noteText->setText(med->getDesc());
                    mainLayTab->addWidget(url);
                    mainLayTab->addWidget(noteText);
                }
            }
        }
    }

    //mainWidTab->setMaximumHeight(800);
    //mainWidTab->setScaledContents(true);
    mainWidTab->setLayout(mainLayTab);
    QScrollArea *b= new QScrollArea();
    b->setWidget(mainWidTab);
    //b->setMaximumHeight(500);
    tab->clear();
    tab->addTab(b, doc->getTitle() );
    tab->setCurrentWidget(b);
    note_actuelle = doc;
    note_actuelle->setOnScreen();
    connect(mainLineTab, SIGNAL(textChanged(QString)), this, SLOT(onTitleChanged(QString)));

}

/*!
 * \brief Cette méthode charge une Image dans l'éditeur
 * \param note
 * Si la note n'a pas été chargée (si elle ne possède pas encore son chemin ni sa description), alors elle est chargée à travers sa méthode load. L'éditeur est ensuite chargé avec ses caractéristiques.
 */
void NotesEditeur::load_image(Note* note)
{
    if (note_actuelle)
        note_actuelle->removeFromScreen();
    cout << "entrée dans load" << endl;
    Image* img = static_cast<Image*>(note);
    if( !note->getLoaded()) {
        cout << "pas chargée"<<endl;
        img->load();
        img->setLoaded();
        cout << "chargée" << endl;
    }
    QWidget* mainWidTab = new QWidget;
    QVBoxLayout* mainLayTab = new QVBoxLayout;
    QLineEdit* lineTitle = new QLineEdit;
    lineTitle->setText(img->getTitle());
    QLineEdit* linePath = new QLineEdit;
    linePath->setText(img->getPath());
    QTextEdit* textDesc = new QTextEdit;
    textDesc->setText(img->getDesc());
    mainLayTab->addWidget(lineTitle);
    mainLayTab->addWidget(linePath);
    mainLayTab->addWidget(textDesc);
    mainWidTab->setLayout(mainLayTab);
    tab->clear();
    tab->addTab(mainWidTab, img->getTitle() );
    tab->setCurrentWidget(mainWidTab);

    QLabel *imageLabel = new QLabel;
    QImage image(img->getPath());
    imageLabel->setPixmap(QPixmap::fromImage(image));
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    tab->addTab(imageLabel, "Image");

    note_actuelle = img;
    note_actuelle->setOnScreen();

    connect( lineTitle, SIGNAL(textChanged(const QString&)), this, SLOT(onTitleChanged(const QString&)) );
    connect( linePath, SIGNAL(textChanged(const QString&)), this, SLOT(onPathChanged(const QString&)) );
    connect( textDesc, SIGNAL(textChanged()), this, SLOT(onDescChanged()) );
}

/*!
 * \brief Cette méthode charge une vidéo dans l'éditeur
 * \param note
 * Si la note n'a pas été chargée (si elle ne possède pas encore son chemin ni sa description), alors elle est chargée à travers sa méthode load. L'éditeur est ensuite chargé avec ses caractéristiques.
 */
void NotesEditeur::load_video(Note* note)
{
    if (note_actuelle)
        note_actuelle->removeFromScreen();
    cout << "entrée dans load" << endl;
    Video* video = static_cast<Video*>(note);
    if( !note->getLoaded()) {
        cout << "pas chargée"<<endl;
        video->load();
        video->setLoaded();
        cout << "chargée" << endl;
    }
    QWidget* mainWidTab = new QWidget;
    QVBoxLayout* mainLayTab = new QVBoxLayout;
    QLineEdit* lineTitle = new QLineEdit;
    lineTitle->setText(video->getTitle());
    QLineEdit* linePath = new QLineEdit;
    linePath->setText(video->getPath());
    QTextEdit* textDesc = new QTextEdit;
    textDesc->setText(video->getDesc());
    mainLayTab->addWidget(lineTitle);
    mainLayTab->addWidget(linePath);
    mainLayTab->addWidget(textDesc);
    mainWidTab->setLayout(mainLayTab);
    tab->clear();
    tab->addTab(mainWidTab, video->getTitle() );
    tab->setCurrentWidget(mainWidTab);

    note_actuelle = video;
    note_actuelle->setOnScreen();

    connect( lineTitle, SIGNAL(textChanged(const QString&)), this, SLOT(onTitleChanged(const QString&)) );
    connect( linePath, SIGNAL(textChanged(const QString&)), this, SLOT(onPathChanged(const QString&)) );
    connect( textDesc, SIGNAL(textChanged()), this, SLOT(onDescChanged()) );
}

/*!
 * \brief Cette méthode charge un audio dans l'éditeur
 * \param note
 * Si la note n'a pas été chargée (si elle ne possède pas encore son chemin ni sa description), alors elle est chargée à travers sa méthode load. L'éditeur est ensuite chargé avec ses caractéristiques.
 */
void NotesEditeur::load_audio(Note* note)
{
    if (note_actuelle)
        note_actuelle->removeFromScreen();
    cout << "entrée dans load" << endl;
    Audio* audio = static_cast<Audio*>(note);
    if( !note->getLoaded()) {
        cout << "pas chargée"<<endl;
        audio->load();
        audio->setLoaded();
        cout << "chargée" << endl;
    }
    QWidget* mainWidTab = new QWidget;
    QVBoxLayout* mainLayTab = new QVBoxLayout;
    QLineEdit* lineTitle = new QLineEdit;
    lineTitle->setText(audio->getTitle());
    QLineEdit* linePath = new QLineEdit;
    linePath->setText(audio->getPath());
    QTextEdit* textDesc = new QTextEdit;
    textDesc->setText(audio->getDesc());
    mainLayTab->addWidget(lineTitle);
    mainLayTab->addWidget(linePath);
    mainLayTab->addWidget(textDesc);
    mainWidTab->setLayout(mainLayTab);
    tab->clear();
    tab->addTab(mainWidTab, audio->getTitle() );
    tab->setCurrentWidget(mainWidTab);

    note_actuelle = audio;
    note_actuelle->setOnScreen();

    connect( lineTitle, SIGNAL(textChanged(const QString&)), this, SLOT(onTitleChanged(const QString&)) );
    connect( linePath, SIGNAL(textChanged(const QString&)), this, SLOT(onPathChanged(const QString&)) );
    connect( textDesc, SIGNAL(textChanged()), this, SLOT(onDescChanged()) );
}

/*!
 * \brief Sauvegarde toutes les notes modifiées.
 *
 * Enlève l'étoile du titre, montrant que la note à été sauvegardée.
 */
void NotesEditeur::save()
{

    NotesManager::getInstance()->saveNotes();
    QString titre = note_actuelle->getTitle();
    tab->setTabText(tab->currentIndex(),titre);
    refreshModel();
}

/*!
 * \brief Ouvre une fenêtre de dialogue permettant de saisir un nouveau Tag
 */
void NotesEditeur::addNewTag()
{
    bool ok;
    QString tagname = QInputDialog::getText(this, tr("Nouveau tag"),
                                              tr("Tag:"), QLineEdit::Normal,
                                            "tag", &ok);
    if (tagname != "")
        TagsManager::getInstance()->addTag(tagname);
}

/*!
 * \brief Créée le gestionnaire de documents et l'affiche
 */
void NotesEditeur::add()
{
    if(note_actuelle && NotesManager::getInstance()->getType(*note_actuelle) == "Document")
    {
        Document* doc = dynamic_cast<Document*>(note_actuelle);
        AddDoc* ad = new AddDoc(doc);
    }
}

/*!
 * \brief Créé le widget permettant l'ajout d'un Tag à une Note
 */
void NotesEditeur::addTag()
{
    if(note_actuelle)
    {
        cout << "addTag" << endl;
        AddTag* t = new AddTag() ;
    }
}

/*!
 * \brief Créé le widget permettant de retirer un Tag à une Note
 */
void NotesEditeur::remTag()
{
    if(note_actuelle)
    {
        RemTag* t = new RemTag() ;
    }
}


/*!
 * \brief Constructeur
 *
 * Ce contructeur créé une nouvelle fenêtre avec la liste des Tags sous la forme de boutons radio.
 */
SupprTag::SupprTag()
{
    vbox = new QVBoxLayout();
    groupbox = new QGroupBox("Quel Tag voulez-vous supprimer ?", &fenetre);
    groupbox->setMinimumWidth(250);
    for(TagsManager::Iterator it = TagsManager::getInstance()->begin(); it != TagsManager::getInstance()->end(); it++)
    {
        cout << (*it)->getTagName().toStdString() << endl;
        QRadioButton *button = new QRadioButton((*it)->getTagName());
        vbox->addWidget(button);
    }
    QPushButton* button = new QPushButton("Ok");
    vbox->addWidget(button);
    groupbox->setLayout(vbox);
    groupbox->move(5, 5);
    //mainLayout->addWidget(groupbox);
    fenetre.setMinimumSize(250, 40 + 30*TagsManager::getInstance()->getSize());

    connect( button, SIGNAL(clicked()), this, SLOT(onSupprTag()));
    fenetre.setWindowIcon(QIcon("../icons/panda.png"));

    fenetre.setWindowTitle("Suppression de Tag");
    fenetre.show();
}


SupprTag::~SupprTag()
{
    delete groupbox;
    delete vbox;
}

/*!
 * \brief Slot du bouton
 *
 * On récupère le Tag sélectionné (En cherchant l'enfant de la GroupBox qui est checked). Cette fonction est robuste au clic sans avoir choisi de Tags (ou quand il n'y en a pas).
 */
void SupprTag::onSupprTag()
{
    QString textRadio = "";
    foreach(QRadioButton *radioButton, groupbox->findChildren<QRadioButton *>())
    {

         if (radioButton->isChecked())
         {
              textRadio = radioButton->text();
         }
    }
    if(textRadio != "")
       {
        TagsManager::getInstance()->removeTag(textRadio);
    }
    fenetre.close();
}

/*!
 * \brief Constructeur
 *
 * Ce contructeur créé une nouvelle fenêtre avec la liste des Tags sous la forme de boutons radio.
 */
AddTag::AddTag()
{
    vbox = new QVBoxLayout();
    groupbox = new QGroupBox("Quel Tag voulez-vous ajouter ?", &fenetre);
    groupbox->setMinimumWidth(250);
    cout << "addTag 2" << endl;
    for(TagsManager::Iterator it = TagsManager::getInstance()->begin(); it != TagsManager::getInstance()->end(); it++)
    {
        cout << (*it)->getTagName().toStdString() << endl;
        QRadioButton *button = new QRadioButton((*it)->getTagName());
        vbox->addWidget(button);
    }
    QPushButton* button = new QPushButton("Ok");
    vbox->addWidget(button);
    groupbox->setLayout(vbox);
    groupbox->move(5, 5);
    //mainLayout->addWidget(groupbox);
    fenetre.setMinimumSize(250, 60 + 25*TagsManager::getInstance()->getSize());

    connect( button, SIGNAL(clicked()), this, SLOT(onAddTag()));
    //connect(actionQuitter, SIGNAL(triggered()), this, SLOT(quit()));

    fenetre.setWindowIcon(QIcon("../icons/panda.png"));
    QString titre = "Ajouter le tag sur ";
    titre += NotesEditeur::getInstance()->getNote()->getTitle();
    fenetre.setWindowTitle(titre);
    fenetre.show();
    //groupbox->addWidget(button);
}

/*!
 * \brief Slot du bouton
 *
 * On récupère le Tag sélectionné (En cherchant l'enfant de la GroupBox qui est checked). Cette fonction est robuste au clic sans avoir choisi de Tags (ou quand il n'y en a pas).
 */
void AddTag::onAddTag()
{
    cout << "on add tag" << endl;
    QString textRadio = "";
    foreach(QRadioButton *radioButton, groupbox->findChildren<QRadioButton *>())
    {

         if (radioButton->isChecked())
         {
              textRadio = radioButton->text();
         }
    }
    cout<< "avant le test" << textRadio.toStdString() <<endl;
    if(textRadio != "")
       {
        cout << NotesEditeur::getInstance()->getNote()->getTitle().toStdString() << endl;
        NotesEditeur::getInstance()->getNote()->addTag(TagsManager::getInstance()->getTag(textRadio));


   }
        cout << "sortie de addTag" << endl;
        fenetre.close();
}

/*!
 * \brief Constructeur
 *
 * Ce contructeur créé une nouvelle fenêtre avec la liste des Tags de la note_actuelle sous la forme de boutons radio.
 */
RemTag::RemTag()
{
    vbox = new QVBoxLayout();
    groupbox = new QGroupBox("Quel Tag voulez-vous supprimer ?", &fenetre);
    groupbox->setMinimumWidth(250);
    for(Note::TagIterator it = NotesEditeur::getInstance()->getNote()->begin(); it != NotesEditeur::getInstance()->getNote()->end(); it++)
    {
        cout << (*it)->getTagName().toStdString() << endl;
        QRadioButton *button = new QRadioButton((*it)->getTagName());
        vbox->addWidget(button);
    }
    QPushButton* button = new QPushButton("Ok");
    vbox->addWidget(button);
    groupbox->setLayout(vbox);
    groupbox->move(5, 5);
    //mainLayout->addWidget(groupbox);
    fenetre.setMinimumSize(250, 60 + 25*NotesEditeur::getInstance()->getNote()->getSize());

    connect( button, SIGNAL(clicked()), this, SLOT(onRemTag()));
    fenetre.setWindowIcon(QIcon("../icons/panda.png"));
    QString titre = "Retirer les tags de ";
    titre += NotesEditeur::getInstance()->getNote()->getTitle();
    fenetre.setWindowTitle(titre);
    fenetre.show();
    //groupbox->addWidget(button);
}

/*!
 * \brief Slot du bouton
 *
 * On récupère le Tag sélectionné (En cherchant l'enfant de la GroupBox qui est checked). Cette fonction est robuste au clic sans avoir choisi de Tags (ou quand il n'y en a pas).
 */
void RemTag::onRemTag()
{
    cout << "on rem tag" << endl;
    QString textRadio;
    foreach(QRadioButton *radioButton, groupbox->findChildren<QRadioButton *>())
    {

         if (radioButton->isChecked())
         {
              textRadio = radioButton->text();
         }
    }
    NotesEditeur::getInstance()->getNote()->remTag(TagsManager::getInstance()->getTag(textRadio));
    fenetre.close();
}

/*!
 * \brief Constructeur
 *
 *
 */
Corbeille::Corbeille()
{
    hbox = new QHBoxLayout();
    vbox = new QVBoxLayout();

    restaure = new QPushButton("Restaurer la note");
    _delete = new QPushButton("Supprimer la note");
    restaureAll = new QPushButton("Restaurer toutes les notes");
    deleteAll = new QPushButton("Supprimer toutes les notes");

    vbox->addWidget(restaure);
    vbox->addWidget(_delete);
    vbox->addWidget(restaureAll);
    vbox->addWidget(deleteAll);
    hbox->addLayout(vbox);
    restaure->setDisabled(true);
    _delete->setDisabled(true);
    vbox2 = new QVBoxLayout();

    modele = new QStandardItemModel;
    view = new QListView;
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setModel(modele);
    //view->header()->hide();

    vbox2->addWidget(view);
    generateModel();

    hbox->addLayout(vbox2);

    fenetre.setLayout(hbox);

    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(onClick(QModelIndex)));
    connect(restaure, SIGNAL(clicked()), this, SLOT(onRestaure()));
    connect(_delete, SIGNAL(clicked()), this, SLOT(onDelete()));
    connect(restaureAll, SIGNAL(clicked()), this, SLOT(onRestaureAll()));
    connect(deleteAll, SIGNAL(clicked()), this, SLOT(onDeleteAll()));
    fenetre.setWindowIcon(QIcon("../icons/corbeille2.png"));
    fenetre.setWindowTitle("Corbeille");
    fenetre.show();



}

/*!
 * \brief Génère un modèle
 * \param item
 *
 * Cette méthode génère un modèle à partir de la liste corbeille du NotesManager.
 */
void Corbeille::generateModel( QStandardItem* item)
{
    QStandardItem* itembis;
    if( !item )

        for( NotesManager::Iterator it = NotesManager::getInstance()->begin_bin(); it != NotesManager::getInstance()->end_bin(); it++ )
        {
            if( (*it)->getTitle() != "" ){
                QString type = NotesManager::getInstance()->getType(*(*it));
                if(type == "Article")
                {
                    itembis = new QStandardItem( QIcon("../icons/article.png"), (*it)->getTitle() );
                    modele->appendRow( itembis );
                }
                else if(type == "Document")
                {
                    itembis = new QStandardItem( QIcon("../icons/document.png"), (*it)->getTitle() );
                    modele->appendRow( itembis );
                }
                else if(type == "Video")
                {
                    itembis = new QStandardItem( QIcon("../icons/video.png"), (*it)->getTitle() );
                    modele->appendRow( itembis );
                }
                else if(type == "Audio")
                {
                    itembis = new QStandardItem( QIcon("../icons/audio.png"), (*it)->getTitle() );
                    modele->appendRow( itembis );
                }
                else if(type == "Image")
                {
                    itembis = new QStandardItem( QIcon("../icons/image.png"), (*it)->getTitle() );
                    modele->appendRow( itembis );
                }

            }
        }

    view->setModel(modele);
}

/*!
 * \brief Rafraichit le modèle
 *
 * Cette méthode permet de rafraichir les modèles. Elle va vider le modèle courrant et générer un nouveau modèle.
 */
void Corbeille::refreshModel()
{
    modele->clear();
    generateModel();
}

/*!
 * \brief Restaure la note cliquée
 */
void Corbeille::onRestaure()
{
    restaure->setDisabled(true);
    _delete->setDisabled(true);
    QString filtre = NotesEditeur::getInstance()->getFilter();
    if (filtre.isEmpty())
        filtre = "";
    NotesManager::getInstance()->restaure(note_actuelle, filtre);
    refreshModel();
    NotesEditeur::getInstance()->refreshModel();
    note_actuelle = 0;
}

/*!
 * \brief Supprime la note cliquée
 */
void Corbeille::onDelete()
{
    restaure->setDisabled(true);
    _delete->setDisabled(true);
    NotesManager::getInstance()->destroy(note_actuelle);
    refreshModel();
    note_actuelle = 0;
}

/*!
 * \brief Restaure toutes les notes de la corbeille
 */
void Corbeille::onRestaureAll()
{
    cout << "on restaure all" << endl;
    QString filtre = NotesEditeur::getInstance()->getFilter();
    if (filtre.isEmpty())
        filtre = "";
    cout << "l'erreur vient de là" << endl;
    NotesManager::getInstance()->restaureAll(filtre);

    cout << "erreur avant model" << endl;
    refreshModel();
    cout << "errer apres model" << endl;
    NotesEditeur::getInstance()->refreshModel();
    note_actuelle = 0;
}

/*!
 * \brief Supprime toutes les notes de la corbeille
 */
void Corbeille::onDeleteAll()
{
    QString filtre = NotesEditeur::getInstance()->getFilter();
    if (filtre.isEmpty())
        filtre = "";
    NotesManager::getInstance()->clearBin();
    refreshModel();
    NotesEditeur::getInstance()->refreshModel();
    note_actuelle = 0;
}

/*!
 * \brief Slot activé lors d'un simple clic sur la vue
 * \param index : index de la note cliquée
 *
 * Charge un pointeur sur la note sélectionnée dans note_actuelle et active les menus correspondants.
 */
void Corbeille::onClick(QModelIndex index)
{
    restaure->setEnabled(true);
    _delete->setEnabled(true);
    QVariant note = modele->data(index, Qt::DisplayRole);
    QString nom = note.toString();
    cout << nom.toStdString() << endl;
    Note* note2 = NotesManager::getInstance()->getBinNote(nom);
    note_actuelle = note2;
    cout << note_actuelle->getTitle().toStdString() << endl;
}


/*!
 * \brief Constructeur
 * \param docu : Document à gérer
 */
AddDoc::AddDoc(Document *docu) : doc(docu)
{
    hbox = new QHBoxLayout();
    vbox = new QVBoxLayout();


    //hbox->addLayout(vbox);

    vbox2 = new QVBoxLayout();

    modele_gauche = new QStandardItemModel;
    view_gauche = new QListView;
    view_gauche->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view_gauche->setModel(modele_gauche);

    vbox->addWidget(view_gauche);
    generateModel_gauche();
    ajouter = new QPushButton("Ajouter la note");
    ajouter->setDisabled(true);
    vbox->addWidget(ajouter);

    hbox->addLayout(vbox);

    modele_droite = new QStandardItemModel;
    view_droite = new QListView;
    view_droite->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view_droite->setModel(modele_droite);

    generateModel_droit();
    vbox2->addWidget(view_droite);



    supprimer = new QPushButton("Retirer la note");
    supprimer->setDisabled(true);
    vbox2->addWidget(supprimer);

    hbox->addLayout(vbox2);

    fenetre.setLayout(hbox);
    connect( ajouter, SIGNAL(clicked()), this, SLOT(onAddDoc()));
    connect(view_gauche, SIGNAL(clicked(QModelIndex)), this, SLOT(onClickGauche(QModelIndex)));
    connect( supprimer, SIGNAL(clicked()), this, SLOT(onRemDoc()));
    connect(view_droite, SIGNAL(clicked(QModelIndex)), this, SLOT(onClickDroit(QModelIndex)));

    fenetre.setWindowIcon(QIcon("../icons/panda.png"));
    QString titre = "Gestion du document : ";
    titre += NotesEditeur::getInstance()->getNote()->getTitle();
    fenetre.setWindowTitle(titre);
    fenetre.setMaximumSize(fenetre.size());
    fenetre.show();
}

/*!
 * \brief Génère un modèle
 * \param item
 *
 * Cette méthode génère un modèle à partir de la liste de Notes du NotesManager, sans les notes déjà inclues dans le Document.
 */
void AddDoc::generateModel_gauche( QStandardItem* item)
{
    bool exist = false;
    QStandardItem* itembis;
    if( !item )
        for( NotesManager::Iterator it = NotesManager::getInstance()->begin(); it != NotesManager::getInstance()->end(); it++ )
        {
            if( (*it)->getTitle() != "" ){
                if (doc->getNb())
                {
                     for(Document::Iterator it2 = doc->begin(); it2 != doc->end(); it2++)
                     {
                         if((*it2)->getTitle() == (*it)->getTitle())
                             exist = true;
                     }
                }
                if (!exist && (*it)!=NotesEditeur::getInstance()->getNote())
             {

                QString type = NotesManager::getInstance()->getType(*(*it));
                if(type == "Article")
                {
                    itembis = new QStandardItem( QIcon("../icons/article.png"), (*it)->getTitle() );
                    modele_gauche->appendRow( itembis );
                }
                else if(type == "Document")
                {
                    itembis = new QStandardItem( QIcon("../icons/document.png"), (*it)->getTitle() );
                    modele_gauche->appendRow( itembis );
                }
                else if(type == "Video")
                {
                    itembis = new QStandardItem( QIcon("../icons/video.png"), (*it)->getTitle() );
                    modele_gauche->appendRow( itembis );
                }
                else if(type == "Audio")
                {
                    itembis = new QStandardItem( QIcon("../icons/audio.png"), (*it)->getTitle() );
                    modele_gauche->appendRow( itembis );
                }
                else if(type == "Image")
                {
                    itembis = new QStandardItem( QIcon("../icons/image.png"), (*it)->getTitle() );
                    modele_gauche->appendRow( itembis );
                }
            }
             exist = false;
            }
        }

    view_gauche->setModel(modele_gauche);
}

/*!
 * \brief Rafraichit le modèle
 *
 * Cette méthode permet de rafraichir les modèles. Elle va vider le modèle courrant et générer un nouveau modèle.
 */
void AddDoc::refreshModel()
{
    modele_gauche->clear();
    generateModel_gauche();
    modele_droite->clear();
    generateModel_droit();
}

/*!
 * \brief Génère un modèle
 * \param item
 *
 * Cette méthode génère un modèle à partir de la liste de Notes du Document.
 */
void AddDoc::generateModel_droit( QStandardItem* item)
{
    QStandardItem* itembis;
    if( !item )
         for(Document::Iterator it = doc->begin(); it != doc->end(); it++)
         {
             if((*it)->getTitle() != "")

             {
                QString type = NotesManager::getInstance()->getType(*(*it));
                if(type == "Article")
                {
                    itembis = new QStandardItem( QIcon("../icons/article.png"), (*it)->getTitle() );
                    modele_droite->appendRow( itembis );
                }
                else if(type == "Document")
                {
                    itembis = new QStandardItem( QIcon("../icons/document.png"), (*it)->getTitle() );
                    modele_droite->appendRow( itembis );
                }
                else if(type == "Video")
                {
                    itembis = new QStandardItem( QIcon("../icons/video.png"), (*it)->getTitle() );
                    modele_droite->appendRow( itembis );
                }
                else if(type == "Audio")
                {
                    itembis = new QStandardItem( QIcon("../icons/audio.png"), (*it)->getTitle() );
                    modele_droite->appendRow( itembis );
                }
                else if(type == "Image")
                {
                    itembis = new QStandardItem( QIcon("../icons/image.png"), (*it)->getTitle() );
                    modele_droite->appendRow( itembis );
                }
            }

            }

    view_droite->setModel(modele_droite);
}

/*!
 * \brief Indique quelle Note est cliquée dans le modèle de gauche
 * \param index
 */
void AddDoc::onClickGauche(QModelIndex index)
{
    ajouter->setEnabled(true);
    cout << "I have been clicked !" << endl;
    QVariant note = modele_gauche->data(index, Qt::DisplayRole);
    QString nom = note.toString();
    cout << nom.toStdString() << endl;
    Note* note2 = NotesManager::getInstance()->getNote(nom);
    note_actuelle_gauche = note2;
    cout << "note _loaded" << note2->getTitle().toStdString() << endl;
    cout << note_actuelle_gauche->getTitle().toStdString() << endl;
    //ajouter->setEnabled(true);
}

/*!
 * \brief Indique quelle Note est cliquée dans le modèle de gauche
 * \param index
 */
void AddDoc::onClickDroit(QModelIndex index)
{
    supprimer->setEnabled(true);
    cout << "I have been clicked !" << endl;
    QVariant note = modele_droite->data(index, Qt::DisplayRole);
    QString nom = note.toString();
    cout << nom.toStdString() << endl;
    Note* note2 = NotesManager::getInstance()->getNote(nom);
    note_actuelle_droite = note2;
    cout << "note _loaded" << note2->getTitle().toStdString() << endl;
    cout << note_actuelle_droite->getTitle().toStdString() << endl;
    //ajouter->setEnabled(true);
}

/*!
 * \brief Ajoute la Note cliquée gauche dans le document.
 */
void AddDoc::onAddDoc()
{
    ajouter->setDisabled(true);
    doc->addNote(*note_actuelle_gauche);
    refreshModel();
    NotesEditeur::getInstance()->load_document(doc);
}

/*!
 * \brief Retire la Note cliquée à droite du Document.
 */
void AddDoc::onRemDoc()
{
    supprimer->setDisabled(true);
    doc->removeNote(note_actuelle_droite);
    refreshModel();
    NotesEditeur::getInstance()->load_document(doc);

}
