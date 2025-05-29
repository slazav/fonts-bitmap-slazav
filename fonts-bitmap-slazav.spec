Name: fonts-bitmap-slazav
Version: 1.0
Release: alt1

Summary: Unicode bitmap collection based on /efont/
License: BSD-3-Clause
Group: System/Fonts/X11 bitmap

Url: http://github.com/slazav/fonts-bitmap-slazav
Source: %name-%version.tar

BuildArch: noarch
BuildRequires: bdftopcf mkfontdir
BuildRequires: rpm-build-fonts
#Requires: chkfontpath

%define fontdir %_bitmapfontsdir/slazav

%description
Unicode bitmap fonts collection, based on /efont/. This font package includes
12, 14, 16, and 24 pixel ISO-10646 fonts.

%prep
%setup -q

%build
%make
mkfontdir

%install
%bitmap_fonts_install slazav

%files -f slazav.files


%changelog
* Thu May 29 2025 Vladislav Zavjalov <slazav@altlinux.org> 1.0-alt1
- first version

