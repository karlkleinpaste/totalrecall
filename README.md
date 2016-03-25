# Total Recall
### shell environment instrumentation, from JPRC
------------------------------------------------

Bash instrumentation to remember everything you do in the shell...forever.

Total Recall ("TR") was a project when I was at Justsystem Pittsburgh
Research Center, which long ago shut its doors (March 2000).  TR was a
general, wide-ranging look at what it would take to remember everything
one does on one's computer system.  The idea was that, disc capacity
having grown so fantastically, it may well no longer make sense to delete
anything.  But if you keep everything, can you put it all to use?  To that
end, we looked at email archival, window system tracking, shell
instrumentation, file revisioning, and other matters.  This is just a part
of one of those topics.

Basically, let's imagine that one wants to recall how one worked with a
certain dataset...10 years ago.

It would be good if one could actually recall the entire windowed-terminal
environment from that time period.  Make sure the record contains a clue
or three about files one was working with at the time.  Combined with any
indexing tool, either for general file archives or (if one sends the
history reports to oneself as email) one's personal email repository, one
can search for the essentials of what one was doing at the time.  Pick up
the right encapsulated piece of shell history -- not just commands, but
environment variables, terminal window geometry, directory stack, the
works -- and re-invoke the whole thing exactly.

There are 3 elements: Two scripts and a tiny C program.

- Recall, the base script which arranges to make periodic
full-environment dumps.
- Reconstruct, the regenerator which eats a shell dump previously made by
Recall and invokes that environment as exactly as possible.
- sti.c, an itty bitty slice of C which puts the TIOCSTI (simulate
terminal input) ioctl to use. This is needed in the restart of the
terminal so that we can tell the shell to load the pseudohistory.

This shell instrumentation depends on use of bash(1) because csh(1) is far
too primitive; "onintr" is nowhere near adequate, and the shell must be
able to trap periodic SIGALRM directly in order to make TR state dumps. It
also depends on use of the at(1) command to schedule those SIGALRM hits.

Compile the trivial "sti" program, and put it and the scripts in your
$PATH. Start making environment dumps with

> . Recall

Notice that's the "dot" command, that is, source it directly into your
shell. Default $RECALL\_INTERVAL is every 30 minutes; a lot of folks seem
to prefer 120 or even 240. Set $RECALL\_MAIL to "yes" if you want the dump
reports mailed to yourself. When you have history state dumps with which
to work, invoke

> Reconstruct *dump-file*

The complete set of TR shell variables is:

- RECALL\_DIRLIST, a colon-separated list of directories to be
searched for recent changes (1 day) when dumps are made (no default --
**you want to set this**).
- RECALL\_DUMP, where dump files are produced (default ~/.TR/dump).
- RECALL\_EXIT, an internally-set indicator that the shell is doing its
final dump on exit.
- RECALL\_HIST, an internally-set value for where dumps were last made.
- RECALL\_INTERVAL, the number of minutes between dumps, scheduled by at(1)
(default 30).
- RECALL\_MAIL, whether to send dumps as email, in which case they are
deleted after sending. Set to 1 or "yes" or "on" (default unset, i.e. off,
dump files are created and left under $RECALL\_DUMP).
- RECALL\_MAXDEPTH, the directory depth to search for recent changes
(default 2).
- RECALL\_NEXT\_ALRM, an internally-set at(1) job number.
- RECALL\_TERM\_PROG, the name of your preferred terminal program.  You
must set this (no default).  Only xterm and gnome-terminal are understood.
- RECALL\_TIMESTAMP, an internal value set only as a dump is created.
- RECALL\_TTY, the internally-set short name of your shell's terminal.

Usage note: Due to the fact that mailx(1) uses base64-encoded content if
it ever notices an ESC character, and many users have ESC as part of
various shell variables (often for use in terminal highlighting), I regret
to say that ESC is mangled into '^' when mailing dumps.  Sorry.  One
glitch I couldn't get around.  Otherwise you'd have dumps you couldn't
actually search readily.

Two aliases are provided:

- atq, a replacement for the real atq(1), which uses real atq to peek into
files to find the actual commands to be executed.
- noRecall, an alias to turn off Recall and dispose of itself.

Start using Recall.  Two years from now, try to remember how you did
something.  You can search your dumps for directories where you think you
were working, or files you were changing, or environment entries that were
interesting, or other processes that you were executing, or...  There's a
lot in a TR dump.  Find that dump, and feed it to Reconstruct.  You'll be
alarmed.

Reconstruct takes 2 options:

- -n: do not destroy the reconstruction source files; they are left in
~/.TR/dump.
- -r: start TR in the reconstructed shell.

I am interested in feedback on the utility of this.

Karl Kleinpaste, 25 March 2016
