/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_UI_VIEWS_DOWNLOAD_BRAVE_DOWNLOAD_ITEM_VIEW_H_
#define BRAVE_BROWSER_UI_VIEWS_DOWNLOAD_BRAVE_DOWNLOAD_ITEM_VIEW_H_

#include "chrome/browser/ui/views/download/download_item_view.h"

// The purpose of this subclass is to add URL origin and lock icon to the
// download item view (shown in the download shelf).
class BraveDownloadItemView : public DownloadItemView {
 public:
  BraveDownloadItemView(download::DownloadItem* download,
                        DownloadShelfView* parent,
                        views::View* accessible_alert);
  ~BraveDownloadItemView() override;

  // views::View:
  gfx::Size CalculatePreferredSize() const override;

 protected:
  // views::View:
  void OnPaint(gfx::Canvas* canvas) override;

 private:
  // Padding between URL lock icon and URL text
  static constexpr int kOriginURLIconRightPadding = 2;

  // These functions calculates the vertical coordinates for each text line
  int GetYForFilenameText() const override;
  int GetYForOriginURLText() const;
  int GetYForStatusText() const;

  // Painting of various download item bits.
  void DrawStatusText(gfx::Canvas* canvas) override;
  void DrawOriginURL(gfx::Canvas* canvas);
  void DrawLockIcon(gfx::Canvas* canvas);

  // Returns origin URL
  const GURL& GetOriginURL();
  base::string16 GetOriginURLText();
  bool IsOriginURLSecure();

  // Returns a square lock icon of |height|.
  gfx::ImageSkia GetLockIcon(int height);

  // Get origin font
  const gfx::FontList& GetOriginTextFontList() const;

  DISALLOW_COPY_AND_ASSIGN(BraveDownloadItemView);
};

#endif
