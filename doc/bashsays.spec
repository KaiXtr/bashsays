Name:           bashsays
Version:        1.0.0
Release:        1%{?dist}
Summary:        Um simples programa que exibe uma mensagem pela tela.

License:        GPL
URL:            https://github.com/KaiXtr/bashsays
Source0:        %{name}-%{version}.tar.gz

Requires:       bash

%description
Um simples programa que exibe uma mensagem
em uma variedade de cores, velocidades e parâmetros.

%prep
%setup -q

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{_bindir}
cp %{name} $RPM_BUILD_ROOT/%{_bindir}
cp bashsays-pt-BR.txt $RPM_BUILD_ROOT/%{_bindir}
cp bashsays-en-US.txt $RPM_BUILD_ROOT/%{_bindir}

%files
%{_bindir}/%{name}
%{_bindir}/bashsays-pt-BR.txt
%{_bindir}/bashsays-en-US.txt

%changelog
* Sun Sep 17 2023 KaiXtr <ewertonmatheus2113@gmail.com>
- primeira versão sendo preparada.