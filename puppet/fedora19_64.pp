group { "puppet":
  ensure => "present",
}

File { owner => 0, group => 0, mode => 0644 }
Exec { path => ['/usr/bin', '/bin', '/usr/sbin', '/sbin'], }

package {'expat-devel':
  ensure  => present,
}

package {'gcc':
  ensure  => present,
}

package {'CUnit-devel':
  ensure  => present,
}

package {'cmake':
  ensure  => present,
}

exec {'bashrc':
  command => 'echo "set -o vi" >>/home/vagrant/.bashrc',
  unless  => 'grep "set *-o" /home/vagrant/.bashrc',
}