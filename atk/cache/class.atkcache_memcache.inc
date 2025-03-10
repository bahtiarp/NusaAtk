<?php
/**
 * This file is part of the Achievo ATK distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * Cache class for memcached (http://www.danga.com/memcached/)
 *
 * @package atk
 * @subpackage cache
 *
 * @copyright (c)2008 Sandy Pleyte
 * @author Sandy Pleyte <sandy@achievo.org>
 * @license http://www.achievo.org/atk/licensing ATK Open Source License
 *
 * @version $Revision: 5898 $
 * $Id: class.atkcache_memcache.inc 6354 2009-04-15 02:41:21Z mvdam $
 */
atkimport("atk.cache.atkcache");

class atkCache_memcache extends atkCache
{
  public $m_memcache;



  /**
   * Constructor
   *
   * @todo Add support for Memcache pools
   */
  public function __construct()
  {
    if(!extension_loaded('memcache'))
    {
      throw new Exception('The memcache extension is not loaded');
    }
    $this->m_memcache = new Memcache;
    $result = @$this->m_memcache->connect(
                $this->getCacheCOnfig('host','localhost'),
                $this->getCacheConfig('port',11211),
                $this->getCacheConfig('timeout',1)
            );
    if (!$result)
    {
      throw new Exception('Can\'t connect to the memcache server');
    }
  }

  /**
   * Inserts cache entry data, but only if the entry does not already exist.
   *
   * @param string $key The entry ID.
   * @param mixed $data The data to write into the entry.
   * @param int $lifetime give a specific lifetime for this cache entry. When $lifetime is false the default lifetime is used.
   * @return bool True on success, false on failure.
   */
  public function add($key,$data,$lifetime=false)
  {
    if (! $this->m_active)
    {
      return false;
    }

    if($lifetime === false) $lifetime = $this->m_lifetime;
    return $this->m_memcache->add($key, $data, null, $lifetime);
  }

  /**
   * Sets cache entry data.
   *
   * @param string $key The entry ID.
   * @param mixed $data The data to write into the entry.
   * @param int $lifetime give a specific lifetime for this cache entry. When $lifetime is false the default lifetime is used.
   * @return bool True on success, false on failure.
   */
  public function set($key,$data,$lifetime=false)
  {
    if (! $this->m_active)
    {
      return false;
    }

    if($lifetime === false) $lifetime = $this->m_lifetime;
    return $this->m_memcache->set($key, $data, null, $lifetime);
  }

  /**
   * Gets cache entry data.
   *
   * @param string $key The entry ID.
   * @return mixed Boolean false on failure, cache data on success.
   */
  public function get($key)
  {
	file_put_contents("memcache.txt",$key."\n",FILE_APPEND);
	  
    if (! $this->m_active)
    {
      return false;
    }
    return $this->m_memcache->get($key);
  }

  /**
   * Deletes a cache entry.
   *
   * @param string $key The entry ID.
   * @return boolean Succes
   */
  public function delete($key)
  {
    if (! $this->m_active)
    {
      return false;
    }
    $this->m_memcache->delete($key);
    return true;
  }

  /**
   * Removes all cache entries.
   *
   * @return boolean Succes
   */
  public function deleteAll()
  {
    if (!$this->m_active)
    {
      return false;
    }
    $this->m_memcache->flush();
    return true;
  }

  /**
   * Get the current cache type
   *
   * @return string atkConfig type
   */
  public function getType()
  {
    return 'memcache';
  }

}

?>