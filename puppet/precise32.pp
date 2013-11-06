group { "puppet":
  ensure => "present",
}

File { owner => 0, group => 0, mode => 0644 }
Exec { path => ['/usr/bin', '/bin', '/usr/sbin', '/sbin'], }
  
exec {'update': command => 'apt-get update', }

package {'CUnit-devel':
  ensure  => present,
  require => Exec['update'],
}

package {'cmake':
  ensure  => present,
  require => Exec['update'],
}

package {'expat-devel':
  ensure  => present,
  require => Exec['update'],
}

exec {'bashrc':
  command => 'echo "set -o vi" >>/home/vagrant/.bashrc',
  unless  => 'grep "set *-o" /home/vagrant/.bashrc',
}