<?php

/**
 * @file
 * Implement an gridstack field.
 */

function gridstack_field_field_widget_info() {
  return array(
    'paragraphs_gridstack' => array(
      'label' => t('Gridstack'),
      'field types' => array('paragraphs'),
      'settings' => array(
        'grid_settings' => array(
          'alwaysShowResizeHandle' => 0,
          'float' => 0,
          'cellHeight' => 60,
          'height' => 0,
          'verticalMargin' => 20,
          'width' => 12,
        ),
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

function gridstack_field_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings']['grid_settings'];
  $form = array();

  if ($widget['type'] == 'paragraphs_gridstack') {
    $form['grid_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Grid settings'),
      '#weight' => 9999,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    $form['grid_settings']['alwaysShowResizeHandle'] = array(
      '#type' => 'checkbox',
      '#title' => t('alwaysShowResizeHandle'),
      '#default_value' => isset($settings['alwaysShowResizeHandle']) ? $settings['alwaysShowResizeHandle'] : 0,
      '#description' => t('If checked the resizing handles are shown even if the user is not hovering over the widget '),
    );
    $form['grid_settings']['float'] = array(
      '#type' => 'checkbox',
      '#title' => t('Float'),
      '#default_value' => isset($settings['float']) ? $settings['float'] : 0,
      '#description' => t('Enable floating widgets'),
    );

    $form['grid_settings']['cellHeight'] = array(
      '#type' => 'textfield',
      '#title' => t('cellHeight'),
      '#default_value' => isset($settings['cellHeight']) ? $settings['cellHeight'] : 60,
      '#size' => 60,
      '#maxlength' => 128,
      '#description' => t('One cell height in pixels'),
    );
    $form['grid_settings']['height'] = array(
      '#type' => 'textfield',
      '#title' => t('height'),
      '#default_value' => isset($settings['height']) ? $settings['height'] : 0,
      '#size' => 60,
      '#maxlength' => 128,
      '#description' => t('Maximum rows amount. Default is 0 which means no maximum rows'),
    );
    $form['grid_settings']['verticalMargin'] = array(
      '#type' => 'textfield',
      '#title' => t('verticalMargin'),
      '#default_value' => isset($settings['verticalMargin']) ? $settings['verticalMargin'] : 20,
      '#size' => 60,
      '#maxlength' => 128,
      '#description' => t('Vertical gap size in pixels'),
    );
    $form['grid_settings']['width'] = array(
      '#type' => 'textfield',
      '#title' => t('width'),
      '#default_value' =>  isset($settings['width']) ? intval($settings['width']) : 12,
      '#size' => 60,
      '#maxlength' => 128,
      '#description' => t('Amount of columns'),
    );
  }

  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function gridstack_field_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  switch ($instance['widget']['type']) {
    case 'paragraphs_gridstack':
      return gridstack_field_field_multiple_value_form($field, $instance, $langcode, $items, $form, $form_state, $delta, $element);
      break;
  }
}

/**
 * Special handling to create form elements for multiple values.
 *
 * Handles generic features for multiple fields:
 * - number of widgets
 * - AHAH-'add more' button
 * - drag-n-drop value reordering
 */
function gridstack_field_field_multiple_value_form($field, $instance, $langcode, $items, &$form, &$form_state, $delta, $original_element) {
  $form['#submit'][] = 'gridstack_field_field_widget_form_submit';

  // Add grid comfig form on node add page.
  if (!isset($form['#node']->nid)) {
    $settings = $instance['widget']['settings']['grid_settings'];

    // Transform string values to int.
    $settings['cellHeight'] = intval($settings['cellHeight']);
    $settings['height'] = intval($settings['height']);
    $settings['verticalMargin'] = intval($settings['verticalMargin']);
    $settings['width'] = intval($settings['width']);

    $form['grid_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Grid settings'),
      '#weight' => 0,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#suffix' => '</div>',
      '#prefix' => '<div id="grid-settings">'
    );

    $form['grid_settings']['alwaysShowResizeHandle'] = array(
      '#type' => 'checkbox',
      '#title' => t('alwaysShowResizeHandle'),
      '#default_value' => $settings['alwaysShowResizeHandle'],
      '#description' => t('If checked the resizing handles are shown even if the user is not hovering over the widget '),
    );
    $form['grid_settings']['float'] = array(
      '#type' => 'checkbox',
      '#title' => t('Float'),
      '#default_value' => $settings['float'],
      '#description' => t('Enable floating widgets'),
    );

    $form['grid_settings']['cellHeight'] = array(
      '#type' => 'textfield',
      '#title' => t('cellHeight'),
      '#default_value' => $settings['cellHeight'],
      '#size' => 60,
      '#maxlength' => 128,
      '#description' => t('One cell height in pixels'),
    );
    $form['grid_settings']['height'] = array(
      '#type' => 'textfield',
      '#title' => t('height'),
      '#default_value' => $settings['height'],
      '#size' => 60,
      '#maxlength' => 128,
      '#description' => t('Maximum rows amount. Default is 0 which means no maximum rows'),
    );
    $form['grid_settings']['verticalMargin'] = array(
      '#type' => 'textfield',
      '#title' => t('verticalMargin'),
      '#default_value' => $settings['verticalMargin'],
      '#size' => 60,
      '#maxlength' => 128,
      '#description' => t('Vertical gap size in pixels'),
    );
    $form['grid_settings']['width'] = array(
      '#type' => 'textfield',
      '#title' => t('width'),
      '#default_value' => $settings['width'],
      '#size' => 60,
      '#maxlength' => 128,
      '#description' => t('Amount of columns'),
    );

    $form['grid_settings']['actions']['save'] = array(
      '#type' => 'button',
      '#value' => t('Save grid'),
      '#name' => 'grid',
      '#validate' => array(),
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'grid_replace_js',
      ),
    );
  }

//  drupal_add_js(array('gridstack_field' => array('settings' => $settings)), array('type' => 'setting'));

  $field_elements = paragraphs_field_multiple_value_form($field, $instance, $langcode, $items, $form, $form_state, $delta, $original_element);
  // Use own theme function.
  $field_elements['#theme'] = 'gridstack_field_field_multiple_value_form';

  // This code needs to be executed only once on first page load.
  if (empty($form_state['values'])) {
    // Install Gridstack library on the page.
    drupal_add_library('gridstack_field', 'gridstack', FALSE);

    // Need to clear cache on every add/edit page load.
    cache_clear_all('grid_items' . session_id(), 'cache_grid', FALSE);
    // Fill in drupal cache and pass settings to our scripts.
    if ($form['nid']['#value']) {
      cache_clear_all('grid_items' . session_id(), 'cache_grid', FALSE);

      $db_data = gridstack_field_load($form['nid']['#value']);
      if (isset($db_data->data)) {
        // Pass gridd settings into our scripts.
        drupal_add_js(array('gridstack_field' => array('settings' => $db_data->data['settings'])), array('type' => 'setting'));
        // Warm up cache.
        cache_set('grid_items' . session_id(), $db_data->data, 'cache_grid', CACHE_PERMANENT);
      }
    }
  }

  return $field_elements;
}

/**
 * @param $form
 * @param $form_state
 *
 * @return array
 */
function grid_replace_js($form, &$form_state) {
  // drupal_html_id() very helpfully ensures that all html IDS are unique
  // on a page. Unfortunately what it doesn't realize is that the IDs
  // we are generating are going to replace IDs that already exist, so
  // this actually works against us.
  if (isset($_POST['ajax_html_ids'])) {
    unset($_POST['ajax_html_ids']);
  }

  // Pass grid settings into our scripts.
  $settings = array();
  $grid_settings = $form_state['complete form']['grid_settings'];
  $settings['alwaysShowResizeHandle'] = $grid_settings['alwaysShowResizeHandle']['#value'];
  $settings['float'] = $grid_settings['float']['#value'];
  $settings['cellHeight'] = intval($grid_settings['cellHeight']['#value']);
  $settings['height'] = intval($grid_settings['height']['#value']);
  $settings['verticalMargin'] = intval($grid_settings['verticalMargin']['#value']);
  $settings['width'] = intval($grid_settings['width']['#value']);

  drupal_add_js(array('gridstack_field' => array('settings' => $settings)), array('type' => 'setting'));

  $data = [];
  $data['items'] = [];
  $data['settings'] = $settings;
  cache_set('grid_items' . session_id(), $data, 'cache_grid', CACHE_PERMANENT);

  $commands = array();
  $commands[] = ajax_command_invoke('#grid-settings','remove');
  $commands[] = ajax_command_invoke('.field-widget-paragraphs-gridstack', 'show');

  $return = array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );

  return $return;
}

/**
 * .
 */
function gridstack_field_field_widget_form_submit($form, &$form_state) {
  if ($form['nid']['#value']) {
    $cached_data = cache_get('grid_items' . session_id());
    gridstack_field_save($form['nid']['#value'], $cached_data->data);

    cache_clear_all('grid_items' . session_id(), 'cache_grid', FALSE);
  }
}

/**
 * Widget form implementation for paragraphs.
 *
 * @return array
 */
function gridstack_field_field_widget_form_build(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $element = paragraphs_field_widget_form_build($form, $form_state, $field, $instance, $langcode, $items, $delta, $element);
  return $element;
}

/**
 * Implements hook_field_formatter_info().
 */
function gridstack_field_field_formatter_info() {
  return array(
    'gridstack_view' => array(
      'label' => t('Gridstack items'),
      'field types' => array('paragraphs'),
      'settings' =>  array(
        'view_mode' => 'full',
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function gridstack_field_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $settings = $display['settings'];
  switch ($display['type']) {
    case 'gridstack_view':

      // Prevent displaying useless markup if we don't have any values.
      if (empty($items)) {
        return $element;
      }
      // Get view mode from entity.
      $display_view_mode = empty($display['settings']['view_mode']) ? 'full' : $display['settings']['view_mode'];
      // Get view mode from field instance (if configured).
      $view_mode = empty($instance['display'][$display_view_mode]['settings']['view_mode']) ? $display_view_mode : $instance['display'][$display_view_mode]['settings']['view_mode'];
      $element['#theme_wrappers'] = array('gridstack_field_items');
      $element['#view_mode'] = $view_mode;

      foreach ($items as $delta => $item) {
        if ($paragraph = paragraphs_field_get_entity($item)) {
          $paragraph->setHostEntity($entity_type, $entity, $langcode);
          if (entity_access('view', 'paragraphs_item', $paragraph)) {
            $element[$delta]['entity'] = $paragraph->view($view_mode);
          }
        }
      }
      break;
  }
  return $element;
}

