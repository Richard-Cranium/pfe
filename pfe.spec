Summary:	Portable Forth Environment
Name:		pfe
Version:	0.30.98
Release:	%{_os}
Serial:		1
Copyright:	LGPL
Group:		Development/Languages
URL:		http://sourceforge.net/projects/pfe
Vendor:		Guido Draheim <guidod@gmx.de>
Source0:	http://ftp1.sourceforge.net/%{name}/%{name}-%{version}.tar.gz
BuildRoot:	/var/tmp/%{name}-%{version}-%{release}

Distribution:	Portable Forth Environment
Packager:	Guido Draheim <guidod@gmx.de>

%package doc
Summary:	PFE Documentation
Group:		Development/Libraries

%package share
Summary:	PFE Modules and Scripts
Group:		Development/Libraries
Requires:	pfe

%package devel
Summary:	PFE Development Headers
Group:		Development/Libraries
Requires:       pfe

%description
The Portable Forth Environment is a Forth Engine completly written in C.
It has a module concept so it can be extended with C modules. It is
completly multithreaded, and implements the DPANS94, the proposed ANSI
standard on Forth.

%description doc
The Portable Forth Environment.
The documentation, mostly autogenerated.

%description share
The Portable Forth Environment.
Some extension modules and scripts.

%description devel
The Portable Forth Environment.
The header file for compiling extension modules for
use by the forth engine, or for compiling applications
that use the forth library as a scripting engine.

The headers are namespace clean, i.e. all symbols
have a prefix to distinguish them from other symbols.

%prep
#'
%setup
CFLAGS="$RPM_OPT_FLAGS" sh configure --prefix=%{_prefix}

%build
make 
make docs

%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}
make install-doc DESTDIR=%{buildroot}

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%clean
rm -rf %{buildroot}

%files
#    %defattr(-,root,root)
%doc Changelog
     %{_prefix}/bin/%{name}
     %{_prefix}/lib/lib%{name}*
%dir %{_prefix}/lib/%{name}
%dir %{_prefix}/share/%{name}

%files doc
#    %config(noreplace) %{prefix}/%{name}.conf
%dir %{_prefix}/doc/%{name}
     %{_prefix}/doc/%{name}/*

%files share
     %{_prefix}/share/%{name}/*
     %{_prefix}/lib/%{name}/*

%files devel
%dir %{_prefix}/include/%{name}
     %{_prefix}/include/%{name}/*
     %{_prefix}/include/%{name}-*


%post doc
echo "PFE Documentation installed"
# %{_prefix}/%{name}-install && /bin/true

