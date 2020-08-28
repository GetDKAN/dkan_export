<?php

/**
 * @file
 * dkan_export.drush.inc.
 */

/**
 * Implements hook_drush_command().
 */
function dkan_export_drush_command() {
  // Run harvest operation on the source passed as argument.
  $items['dkan-export-users'] = array(
    'description' => 'Export users to migrate into a DKAN2 site.',
    'callback' => 'dkan_export_drush_export',
    'arguments' => array(),
    'options' => array(
      'only-active' => 'Only include active users, skip blocked users.',
      'skip-uuids' => 'Do not include users authored datasets\' uuids.',
      'destination-file' => 'Specify a destination file for the output, do not output the result to stdout.'
    ),
  );

  return $items;
}

/**
 * Export users commands.
 */
function dkan_export_drush_export() {
  // Collect options.
  $onlyactive = drush_get_option('only-active');

  if (empty($onlyactive)) {
    $onlyactive = FALSE;
  } else {
    $onlyactive = TRUE;
  }

  $skip_uuids = drush_get_option('skip-uuids');

  if (empty($skip_uuids)) {
    $skip_uuids = FALSE;
  } else {
    $skip_uuids = TRUE;
  }

  $destination_file = drush_get_option('destination-file');

  if (empty($destination_file)) {
    $destination_file = '';
  }

  $query = db_select('users', 'u');
  $query->fields('u', array('uid', 'name', 'status'));

  $query->groupBy('uid')
        ->orderBy('uid');

  if (!$skip_uuids) {
    $query->addExpression('GROUP_CONCAT(n.uuid)', 'uuids');
    /* Limit to only datasets since only those are exported in the data.json. */
    $query->leftjoin('node', 'n', 'u.uid = n.uid AND n.type = \'dataset\'');
  }

  $query->leftjoin('field_data_field_about', 'a', 'a.entity_type = \'user\' AND a.entity_id = u.uid');

  $query->addField('u', 'mail', 'email');
  $query->addField('u', 'created');
  $query->addField('u', 'access', 'last_access');
  $query->addField('u', 'login', 'last_login');
  $query->addField('a', 'field_about_value', 'about');

  // Exclude anonymous.
  $query->condition('u.uid', 0, '<>');
  // Exclude root.
  $query->condition('u.uid', 1, '<>');

  // Add status condition based on option.
  if ($onlyactive) {
    $query->condition('u.status', 1);
  }

  $result = $query->execute();

  $data = loft_data_grids_export_data();

  foreach ($result as $row) {
    foreach ($row as $key => $value) {
      $data->add($key, $value);
    }
    $data->next();
  }

  $data->normalize('');

  $exporter = loft_data_grids_exporter($data, 'CSVExporter', $destination_file);
  $output = $exporter->export();

  if (!empty($destination_file)) {
    file_unmanaged_save_data($output, $destination_file);
  }else {
    echo $output;
  }
}
