/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/views/download/brave_download_item_view.h"

#include "base/strings/utf_string_conversions.h"
#include "brave/app/vector_icons/vector_icons.h"
#include "chrome/browser/ui/views/download/download_shelf_view.h"
#include "components/vector_icons/vector_icons.h"
#include "content/public/common/origin_util.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/paint_vector_icon.h"
#include "ui/gfx/text_elider.h"
#include "ui/gfx/text_utils.h"
#include "third_party/skia/include/core/SkColor.h"

using download::DownloadItem;

namespace {

// The vertical distance between the item's visual upper bound (as delineated
// by the separator on the right) and the edge of the shelf.
constexpr int kTopBottomPadding = 6;

// The minimum vertical padding above and below contents of the download item.
// This is only used when the text size is large.
constexpr int kMinimumVerticalPadding = 2 + kTopBottomPadding;

// The normal height of the item which may be exceeded if text is large.
constexpr int kDefaultHeight = 48;

// Lock icon color.

constexpr SkColor kDownloadUnlockIconColor = SkColorSetRGB(0xC6, 0x36, 0x26);

} // namespace

BraveDownloadItemView::BraveDownloadItemView(DownloadItem* download,
                                             DownloadShelfView* parent,
                                             views::View* accessible_alert)
    : DownloadItemView(download, parent, accessible_alert) {}

BraveDownloadItemView::~BraveDownloadItemView() {}

// View overrides

gfx::Size BraveDownloadItemView::CalculatePreferredSize() const {
  // Call super to get the width.
  gfx::Size size = DownloadItemView::CalculatePreferredSize();
  // Calculate the height accounting for the extra line and with better line
  // spacing.
  int child_height = font_list_.GetHeight() + kVerticalTextPadding +
                     GetOriginTextFontList().GetHeight() + kVerticalTextPadding +
                     status_font_list_.GetHeight();
  if (IsShowingWarningDialog()) {
    child_height =
        std::max({child_height, GetButtonSize().height(), kWarningIconSize});
  }
  size.set_height(
      std::max(kDefaultHeight, 2 * kMinimumVerticalPadding + child_height));
  return size;
}

void BraveDownloadItemView::OnPaint(gfx::Canvas* canvas) {
  DownloadItemView::OnPaint(canvas);
  DrawOriginURL(canvas);
}

// Positioning routines

int BraveDownloadItemView::GetYForFilenameText() const {
  int y = DownloadItemView::GetYForFilenameText();
  y -= (kVerticalTextPadding + GetOriginTextFontList().GetHeight()) / 2;
  return y;
}

int BraveDownloadItemView::GetYForOriginURLText() const {
  int y = GetYForFilenameText();
  y += (font_list_.GetHeight() + kVerticalTextPadding);
  return y;
}

int BraveDownloadItemView::GetYForStatusText() const {
  int y = GetYForOriginURLText();
  y += (GetOriginTextFontList().GetHeight() + kVerticalTextPadding);
  return y;
}

// Drawing routines

void BraveDownloadItemView::DrawStatusText(gfx::Canvas* canvas) {
  if (status_text_.empty() || IsShowingWarningDialog())
    return;

  int mirrored_x = GetMirroredXWithWidthInView(
      kStartPadding + DownloadShelf::kProgressIndicatorSize +
          kProgressTextPadding,
      kTextWidth);
  canvas->DrawStringRect(status_text_, status_font_list_, GetDimmedTextColor(),
                         gfx::Rect(mirrored_x, GetYForStatusText(), kTextWidth,
                                   status_font_list_.GetHeight()));
}

void BraveDownloadItemView::DrawOriginURL(gfx::Canvas* canvas) {
  if (IsShowingWarningDialog())
    return;

  int x = kStartPadding + DownloadShelf::kProgressIndicatorSize +
          kProgressTextPadding;
  int text_width = kTextWidth;
 
  if (!IsOriginURLSecure()) {
    DrawLockIcon(canvas);
    int dx = GetOriginTextFontList().GetBaseline() + kOriginURLIconRightPadding;
    x += dx;
    text_width -= dx;
  }

  base::string16 originURL = gfx::ElideText(
      GetOriginURLText(), GetOriginTextFontList(), text_width, gfx::ELIDE_TAIL);
  int mirrored_x = GetMirroredXWithWidthInView(x, text_width);

  canvas->DrawStringRect(
      originURL, GetOriginTextFontList(),
      enabled() ? GetTextColor() : GetDimmedTextColor(),
      gfx::Rect(mirrored_x, GetYForOriginURLText(), text_width,
                GetOriginTextFontList().GetHeight()));
  }

void BraveDownloadItemView::DrawLockIcon(gfx::Canvas* canvas) {
  if (IsShowingWarningDialog())
    return;

  int mirrored_x = GetMirroredXWithWidthInView(
      kStartPadding + DownloadShelf::kProgressIndicatorSize +
          kProgressTextPadding,
      kTextWidth);

  // Get lock icon of the font cap height size
  int dy = GetOriginTextFontList().GetBaseline();
  int y = GetYForOriginURLText() + (GetOriginTextFontList().GetHeight() - dy) / 2;
  canvas->DrawImageInt(GetLockIcon(dy), mirrored_x, y);
}

// Origin URL routines

const GURL& BraveDownloadItemView::GetOriginURL() {
  if (!download()->GetURL().is_empty())
    return download()->GetURL();
  return download()->GetOriginalUrl();
}

bool BraveDownloadItemView::IsOriginURLSecure() {
  return content::IsOriginSecure(GetOriginURL());
}

base::string16 BraveDownloadItemView::GetOriginURLText() {
  const std::string origin = GetOriginURL().GetOrigin().spec();
  return base::UTF8ToUTF16(origin);
}

// Get lock icon from vector icons
gfx::ImageSkia BraveDownloadItemView::GetLockIcon(int height) {
  return gfx::CreateVectorIcon(kDownloadUnlockIcon, height,
    kDownloadUnlockIconColor);
}

const gfx::FontList& BraveDownloadItemView::GetOriginTextFontList() const {
  return (status_text_.empty() ? font_list_ : status_font_list_);
}
