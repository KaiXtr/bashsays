Name:           bashsays
Version:        0.0.1
Release:        1%{?dist}
Summary:        A program that displays a scrolling message.

License:        GPL
URL:            https://github.com/KaiXtr/bashsays
Source0:        %{name}-%{version}.tar.gz

Requires:       bash

%description
A simple program that displays a scrolling message
on a variety of colors, speed and parameters.

%prep
%setup -q

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{_bindir}
cp %{name} $RPM_BUILD_ROOT/%{_bindir}

%files
%{_bindir}/%{name}

%changelog
* Sun Sep 17 2023 KaiXtr <ewertonmatheus2113@gmail.com>
- primeira versão sendo preparada.
