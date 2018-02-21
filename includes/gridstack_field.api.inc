<?php

/**
 * @file
 * Contains CRUD operations for SimpleCrop module.
 */

/**
 * Create/update gridstack data.
 *
 * @param $uri
 *   URI of image.
 *
 * @param $data
 *   Data that contains gridstack data.
 */
function gridstack_field_save($nid, $data) {

  $gridstack = new stdClass();
  $gridstack->nid = $nid;
  $gridstack->data = $data;
  // Save gridstack data into the database.
  db_merge('gridstack')
    ->key(array('nid' => $gridstack->nid))
    ->fields(array('data' => serialize($gridstack->data)))
    ->execute();
}

/**
 * Load gridstack setting by nid.
 *
 * @param $nid
 *   Nid of node.
 *
 * @return mixed
 */
function gridstack_field_load($nid, $reset = FALSE) {

  $cache = &drupal_static(__FUNCTION__);
  if (!isset($cache[$nid]) || $reset) {

    $cache[$nid] = db_select('gridstack')
      ->fields('gridstack')
      ->condition('nid', $nid)
      ->execute()
      ->fetchObject();

    if (!empty($cache[$nid])) {

      // Crop data stores serialized so we have to unserialize it before using.
      $cache[$nid]->data = unserialize($cache[$nid]->data);

    }
  }

  return $cache[$nid];
}

/**
 * Delete gridstack by file ID.
 *
 * @param $nid
 *   NID of node.
 */
function gridstack_field_delete($nid) {

  // Load gridstack before deletion to ensure that it exists.
  $gridstack = gridstack_field_load($nid);
  if (!empty($gridstack)) {

    // Delete gridstack settings from table by its nid.
    db_delete('gridstack')
      ->condition('nid', $gridstack->nid)
      ->execute();
  }
}
