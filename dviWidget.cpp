// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
//
// Class: DVIWidget
//
// Widget for displaying TeX DVI files.
// Part of KDVI- A previewer for TeX DVI files.
//
// (C) 2004-2005 Wilfried Huss, Stefan Kebekus
// Distributed under the GPL
//

#include <config.h>

#include "dviWidget.h"
#include "documentPageCache.h"
#include "hyperlink.h"
#include "kvs_debug.h"
#include "renderedDviPagePixmap.h"

#include <klocale.h>


DVIWidget::DVIWidget(PageView* sv, DocumentPageCache* cache, const char* name)
  : DocumentWidget(sv, cache, name)
{
}


void DVIWidget::mousePressEvent(QMouseEvent* e)
{
  // Safety check
  if (!pageNumber.isValid())
    return;

  // Get a pointer to the page contents
  RenderedDviPagePixmap* pageData = dynamic_cast<RenderedDviPagePixmap*>(documentCache->getPage(pageNumber));
  if (pageData == 0)
  {
    kdDebug(kvs::dvi) << "DVIWidget::mousePressEvent(...) pageData for page #" << pageNumber << " is empty" << endl;
    return;
  }

  // Check if the mouse is pressed on a source-hyperlink
  // source hyperlinks can be invoked with the Middle Mousebutton or alternatively
  // with Control+Left Mousebutton
  if ((e->button() == MidButton || (e->button() == LeftButton && (e->state() & ControlButton)))
      && (pageData->sourceHyperLinkList.size() > 0))
  {
    int minIndex = 0;
    int minimum = 0;

    for(unsigned int i=0; i<pageData->sourceHyperLinkList.size(); i++)
    {
      if (pageData->sourceHyperLinkList[i].box.contains(inverseMap(e->pos())))
      {
        emit(SRCLink(pageData->sourceHyperLinkList[i].linkText, inverseMap(e->pos()), this));
        e->accept();
        return;
      }
      // Remember the closest source link
      QPoint center = pageData->sourceHyperLinkList[i].box.center();
      int dx = center.x() - e->pos().x();
      int dy = center.y() - e->pos().y();
      if (dx*dx + dy*dy < minimum || i == 0)
      {
        minIndex = i;
        minimum = dx*dx + dy*dy;
      }
    }
    // If the mouse pointer is not exactly inside a source link, jump to the closest target.
    emit(SRCLink(pageData->sourceHyperLinkList[minIndex].linkText, inverseMap(e->pos()), this));
    e->accept();
  }

  // Call implementation from parent
  DocumentWidget::mousePressEvent(e);
}


void DVIWidget::mouseMoveEvent(QMouseEvent* e)
{
  // Safety check
  if (!pageNumber.isValid())
    return;

  // Call the standard implementation
  DocumentWidget::mouseMoveEvent(e);

  // Analyze the mouse movement only if no mouse button was pressed
  if ( e->state() == 0 ) {
    // Get a pointer to the page contents
    RenderedDviPagePixmap* pageData = dynamic_cast<RenderedDviPagePixmap*>(documentCache->getPage(pageNumber));
    if (pageData == 0) {
      kdDebug(kvs::dvi) << "DVIWidget::mouseMoveEvent(...) pageData for page #" << pageNumber << " is empty" << endl;
      return;
    }

    // Check if the cursor hovers over a sourceHyperlink.
    for(unsigned int i=0; i<pageData->sourceHyperLinkList.size(); i++) {
      if (pageData->sourceHyperLinkList[i].box.contains(e->pos())) {
        clearStatusBarTimer.stop();

        // The macro-package srcltx gives a special like "src:99 test.tex"
        // while MikTeX gives "src:99test.tex". KDVI tries
        // to understand both.
        QString cp = pageData->sourceHyperLinkList[i].linkText;
        int max = cp.length();
        int i;
        for(i=0; i<max; i++)
          if (cp[i].isDigit() == false)
            break;

        emit setStatusBarText( i18n("line %1 of %2").arg(cp.left(i)).arg(cp.mid(i).simplifyWhiteSpace()) );
        return;
      }
    }
  }
}


#include "dviWidget.moc"
