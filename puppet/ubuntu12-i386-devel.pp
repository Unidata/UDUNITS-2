group { "puppet":
  ensure => "present",
}

File { owner => 0, group => 0, mode => 0644 }
Exec { path => ['/usr/bin', '/bin', '/usr/sbin', '/sbin'], }

package {'make':
  ensure  => present,
}

package {'libcunit1':
  ensure  => present,
}

package {'libcunit1-dev':
  ensure  => present,
}

package {'cmake':
  ensure  => present,
}

package {'expat':
  ensure  => present,
}

package {'libexpat-dev':
  ensure  => present,
}

exec {'bashrc':
  command => 'echo "set -o vi" >>/home/vagrant/.bashrc',
  unless  => 'grep "set *-o" /home/vagrant/.bashrc',
}