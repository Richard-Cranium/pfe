%{!?dist: %{?suse_version: %define dist suse}}
%define variant forth
Summary:    Portable Forth Environment
Name:	    pfe
Version:    0.33.71
Release:    1%{dist}
License:    LGPL
Group:	    Development/Languages
URL:	    http://sourceforge.net/projects/pfe
Vendor:	    Guido U. Draheim <guidod@gmx.de>
Source0:    http://prdownloads.sf.net/%{name}/%{name}-%{version}.tar.bz2
BuildRoot:  %{_tmppath}/%{name}-%{variant}-%{version}-%{release}
Packager:	Guido U. Draheim <guidod@gmx.de>
BuildRequires: ncurses-devel
BuildRequires: pkg-config

%package %{variant}
Summary:	PFE Forth
Group:		Development/Languages
Provides:       pfe

%package docs
Summary:	PFE Documentation
Group:		Development/Libraries
Provides:       pfe-doc
BuildRequires:  python
BuildRequires:  xmlto

%package %{variant}-devel
Summary:	PFE Development Headers
Group:		Development/Libraries
Requires:       pfe-%{variant}
Provides:       pfe-devel

# newest opensuse ships with texlive that depends on zziplib
%if 0%{opensuse_bs}
BuildRequires: zziplib
%endif

%description
The Portable Forth Environment is a Forth Engine completly written in C.
It has a module concept so it can be extended with C modules. It is
completly multithreaded, and implements the DPANS94, the proposed ANSI
standard on Forth.

Available rpmbuild rebuild options:
--with : regs fast call sbr modules testmodule

%description %{variant}
The Portable Forth Environment is a Forth Engine completly written in C.
Its modules implement the DPANS94, the proposed ANSI standard on Forth.

%description docs
The Portable Forth Environment.
The documentation, mostly autogenerated. (as for http://PFE.sf.net)

%description %{variant}-devel
The Portable Forth Environment.
The header file for compiling extension modules for
use by the forth engine, or for compiling applications
that use the forth library as a scripting engine.

The headers are namespace clean, i.e. all symbols
have a prefix to distinguish them from other symbols.

%prep
%setup
# % configure - the configure macro has weird interactions with pfe builds
export pfemoduleslibdir=%{_libdir}
CFLAGS="$RPM_OPT_FLAGS" sh configure \
       --prefix=%{_prefix} \
        --bindir=%{_bindir} \
        --sbindir=%{_sbindir} \
        --sysconfdir=%{_sysconfdir} \
        --datadir=%{_datadir} \
        --includedir=%{_includedir} \
        --libdir=%{_libdir} \
        --libexecdir=%{_libexecdir} \
        --localstatedir=%{_localstatedir} \
        --sharedstatedir=%{_sharedstatedir} \
        --mandir=%{_mandir} \
        --infodir=%{_infodir} \
    %{?_with_call: %{!?_with_sbr: --with-call-threading}} \
    %{?_with_call: %{?_with_sbr: --with-sbr-call-threading}} \
    %{!?_with_call: %{?_with_sbr: --with-sbr-threading}} \
    %{?_with_regs: --with-regs=all} \
    %{?_with_fast: --with-static-regs} \
    %{?_without_modules: --without-modules} \
    %{?_without_testmodule: --without-testmodule}

%build
export SHOWBADTESTOUT=y
make
make check
make mans
make docs

%install
rm -rf %{buildroot}
mkdir  %{buildroot}
make install DESTDIR=%{buildroot}
make install-mans DESTDIR=%{buildroot}
make install-docs DESTDIR=%{buildroot}
rm %{buildroot}%{_bindir}/pfe-exec
rm %{buildroot}%{_bindir}/pfe
rmdir %{buildroot}%{_infodir}

echo ==========================
cat %buildroot%_libdir/pkgconfig/*.pc
echo ==========================

%clean
rm -rf %{buildroot}

%post %{variant}
/sbin/ldconfig
for bin in pfe-exec pfe ; do
test ! -f %{_bindir}/$bin || rm %{_bindir}/$bin
for variant in forth call calls fast fastest; do
if test -f %{_bindir}/$bin-$variant
then (cd %{_bindir} && ln -s $bin-$variant $bin) ; break ; fi
done ; test -f %{_bindir}/$bin || exit $?; done

%postun %{variant}
/sbin/ldconfig
for bin in pfe-config pfe-exec pfe ; do
test ! -f %{_bindir}/$bin || rm %{_bindir}/$bin
for variant in forth call calls fast fastest; do
if test -f %{_bindir}/$bin-$variant
then (cd %{_bindir} && ln -s $bin-$variant $bin) ; break ; fi
done done

%files %{variant}
     %defattr(-,root,root)
%doc ChangeLog
     %{_bindir}/%{name}*
     %{_libdir}/lib%{name}*.so.*
%dir %{_libdir}/%{name}-%{variant}
     %{_libdir}/%{name}-%{variant}/*.so
%dir %{_datadir}/%{name}
     %{_datadir}/%{name}/*
     %{_mandir}/man1/*

%files docs
     %defattr(-,root,root)
%dir %{_infodir}/../doc/%{name}
     %{_infodir}/../doc/%{name}/*

%post docs
echo "PFE Documentation installed"
: scrollkeeper-install && /bin/true

%files %{variant}-devel
     %defattr(-,root,root)
     %{_bindir}/*-exec-*
     %{_mandir}/man3/*
%dir %{_prefix}/include/%{name}
     %{_prefix}/include/%{name}/*
     %{_libdir}/lib%{name}*.so
     %{_libdir}/lib%{name}*.la
     %{_libdir}/lib%{name}*.a
     %{_libdir}/%{name}-%{variant}/*.la
     %{_libdir}/%{name}-%{variant}/*.a
%dir %{_libdir}/%{name}-%{variant}/renamed
     %{_libdir}/%{name}-%{variant}/renamed/*
     %{_libdir}/pkgconfig/*.pc
