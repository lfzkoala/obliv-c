#ifdef WIN32
#	include <stdlib.h>
#endif
#if __STDC__
#include <stdarg.h>
#else /*__STDC__*/
#include <varargs.h>
#endif /*__STDC__*/
#ifdef __VMS
#  include <errno.h>
#endif
#include <sys/errno.h>


char* progname = "vararg3";
int showmessages = 1;


void
pm_error( char* format, ... )
    {
    va_list args;

    va_start( args, format );

    fprintf( stderr, "%s: ", THEPROGNAME );
    (void) vfprintf( stderr, format, args );
    fputc( '\n', stderr );
    va_end( args );
    exit( 1 );
    }


/* Portable mini-vfprintf, for systems that don't have either vfprintf or
** _doprnt.  This depends only on fprintf.  If you don't have fprintf,
** you might consider getting a new stdio library.
*/

int
vfprintf( stream, format, args )
    FILE* stream;
    char* format;
    va_list args;
    {
    int n;
    char* ep;
    char fchar;
    char tformat[512];
    int do_long;
    int i;
    long l;
    unsigned u;
    unsigned long ul;
    char* s;
    double d;

    n = 0;
    while ( *format != '\0' )
	{
	if ( *format != '%' )
	    { /* Not special, just write out the char. */
	    (void) putc( *format, stream );
	    ++n;
	    ++format;
	    }
	else
	    {
	    do_long = 0;
	    ep = format + 1;

	    /* Skip over all the field width and precision junk. */
	    if ( *ep == '-' )
		++ep;
	    if ( *ep == '0' )
		++ep;
	    while ( isdigit( *ep ) )
		++ep;
	    if ( *ep == '.' )
		{
		++ep;
		while ( isdigit( *ep ) )
		    ++ep;
		}
	    if ( *ep == '#' )
		++ep;
	    if ( *ep == 'l' )
		{
		do_long = 1;
		++ep;
		}

	    /* Here's the field type.  Extract it, and copy this format
	    ** specifier to a temp string so we can add an end-of-string.
	    */
	    fchar = *ep;
	    (void) strncpy( tformat, format, ep - format + 1 );
	    tformat[ep - format + 1] = '\0';

	    /* Now do a one-argument fprintf with the format string we have
	    ** isolated.
	    */
	    switch ( fchar )
		{
		case 'd':
		if ( do_long )
		    {
		    l = va_arg( args, long );
		    n += fprintf( stream, tformat, l );
		    }
		else
		    {
		    i = va_arg( args, int );
		    n += fprintf( stream, tformat, i );
		    }
		break;

	        case 'o':
	        case 'x':
	        case 'X':
	        case 'u':
		if ( do_long )
		    {
		    ul = va_arg( args, unsigned long );
		    n += fprintf( stream, tformat, ul );
		    }
		else
		    {
		    u = va_arg( args, unsigned );
		    n += fprintf( stream, tformat, u );
		    }
		break;

	        case 'c':
		i = (char) va_arg( args, int );
		n += fprintf( stream, tformat, i );
		break;

	        case 's':
		s = va_arg( args, char* );
		n += fprintf( stream, tformat, s );
		break;

	        case 'e':
	        case 'E':
	        case 'f':
	        case 'g':
	        case 'G':
		d = va_arg( args, double );
		n += fprintf( stream, tformat, d );
		break;

	        case '%':
		(void) putc( '%', stream );
		++n;
		break;

		default:
		return -1;
		}

	    /* Resume formatting on the next character. */
	    format = ep + 1;
	    }
	}
    return nc;
    }

