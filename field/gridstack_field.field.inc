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
          'width' => 12,
          'alwaysShowResizeHandle' => false,
          'cellHeight' => 60,
          'verticalMargin' => 20,
          'height' => 0,
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

    $form['grid_settings']['width'] = array(
      '#type' => 'textfield',
      '#title' => t('width'),
      '#default_value' =>  (isset($settings['width'])) ? $settings['width'] : 12,
      '#size' => 60,
      '#maxlength' => 128,
    );
    $form['grid_settings']['alwaysShowResizeHandle'] = array(
      '#type' => 'textfield',
      '#title' => t('alwaysShowResizeHandle'),
      '#default_value' => (isset($settings['alwaysShowResizeHandle'])) ? $settings['alwaysShowResizeHandle'] : 'false',
      '#size' => 60,
      '#maxlength' => 128,
    );
    $form['grid_settings']['cellHeight'] = array(
      '#type' => 'textfield',
      '#title' => t('cellHeight'),
      '#default_value' => (isset($settings['cellHeight'])) ? $settings['cellHeight'] : 60,
      '#size' => 60,
      '#maxlength' => 128,
    );
    $form['grid_settings']['verticalMargin'] = array(
      '#type' => 'textfield',
      '#title' => t('verticalMargin'),
      '#default_value' => (isset($settings['verticalMargin'])) ? $settings['verticalMargin'] : 20,
      '#size' => 60,
      '#maxlength' => 128,
    );
    $form['grid_settings']['height'] = array(
      '#type' => 'textfield',
      '#title' => t('height'),
      '#default_value' => (isset($settings['height'])) ? $settings['height'] : 0,
      '#size' => 60,
      '#maxlength' => 128,
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
  $field_elements = paragraphs_field_multiple_value_form($field, $instance, $langcode, $items, $form, $form_state, $delta, $original_element);
  // Use own theme function.
  $field_elements['#theme'] = 'gridstack_field_field_multiple_value_form';
  // Install Gridstack library on the page.
  drupal_add_library('gridstack_field', 'gridstack', FALSE);

  // .
  if ($form['nid']['#value']) {
    cache_clear_all('grid_items' . session_id(), 'cache', FALSE);

    $db_data = gridstack_field_load($form['nid']['#value']);
    cache_set('grid_items' . session_id(), $db_data->data, 'cache', CACHE_PERMANENT);
  }
  return $field_elements;
}

/**
 * .
 */
function gridstack_field_field_widget_form_submit($form, &$form_state) {
  if ($form['nid']['#value']) {
    $cached_data = cache_get('grid_items' . session_id());
    gridstack_field_save($form['nid']['#value'], $cached_data->data);

    cache_clear_all('grid_items' . session_id(), 'cache', FALSE);
  }
}

/**
 * Widget form implementation for paragraphs.
 *
 * @param $form
 * @param $form_state
 * @param $field
 * @param $instance
 * @param $langcode
 * @param $items
 * @param $delta
 * @param $element
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

