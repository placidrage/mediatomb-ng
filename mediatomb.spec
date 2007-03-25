%define name mediatomb 
%define version 0.9.0
%define release 1

Version: %{version}
Summary: UPnP AV MediaServer 
Name: %{name}
Release: %{release}
License: GPL
Group: Applications/Multimedia
Source: %{name}-%{version}.tar.gz
URL: http://mediatomb.cc
Buildroot: %{_tmppath}/%{name}-%{version}-buildroot 
BuildRequires: sqlite-devel => 3
BuildRequires: file

%description
MediaTomb - UPnP AV Mediaserver for Linux.

%prep 

%setup -q

%build
%configure

make

%install
rm -rf $RPM_BUILD_ROOT

install -D -m0755 scripts/mediatomb-service-fedora %{buildroot}%{_initrddir}/mediatomb

%makeinstall

%clean
rm -rf $RPM_BUILD_ROOT

%post
chkconfig --add mediatomb

%preun
chkconfig --del mediatomb

%files
%defattr(-,root,root)
%doc README README.UTF_8 AUTHORS ChangeLog COPYING INSTALL doc/doxygen.conf
%doc doc/scripting.txt doc/scripting_utf8.txt
%{_bindir}/mediatomb
%{_datadir}/%{name}/
%{_mandir}/man1/*
%{_initrddir}/mediatomb

%changelog
* Sun Mar 25 2007 Sergey Bostandzhyan <jin@mediatomb.cc> -0 0.9.0-1
- Synced with the new script naming and adjusted for the release,
  added man page.
* Mon Feb 26 2007 Sergey Bostandzhyan <jin@mediatomb.cc>
- Removed some files that were no longer needed.
* Wed Sep  7 2005 Sergey Bostandzhyan <jin@mediatomb.cc>
- Removed some buildrequires, our configure script should handle different
  scenarios itself.
* Wed Jun 15 2005 Sergey Bostandzhyan <jin@mediatomb.cc>
- Added init.d script + chkconfig
* Thu Apr 14 2005 Sergey Bostandzhyan <jin@mediatomb.cc>
- Initial release

