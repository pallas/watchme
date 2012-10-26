watchme
=======

Takes a list of files and directories to watch as arguments, watches them.
Exits on modification or deletion.  Useful for automatically running make,
or other processes that produce artifacts.

	while ./watchme . ; do make clean all ; done

Requires one of

	* libgamin	http://people.gnome.org/~veillard/gamin/
	* libfam	http://oss.sgi.com/projects/fam/
