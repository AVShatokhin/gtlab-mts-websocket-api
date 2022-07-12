# gtlab-mts-websocket-api

Реализация WEBSOCKET-API между UI (by МТС) и BACK-END (by GTLAB.Диагностика)

Обмен сообщениями происходит через WebSocket, сообщения кодируются в JSON и должны соответствовать следующим схемам. Схемы представлены в формате [JSONSchema](https://json-schema.org/understanding-json-schema/index.html).

## Строка подключения

Строка подключения к серверу имеет следующий формат: `ws://${HOST}:${PORT}/api/v${VERSION}`, где

* `${HOST}` -- имя хоста сервера
* `${PORT}` -- порт сервера
* `${VERSION}` -- версия API сервера

при попытке открыть соединение с неподдерживаемой версией протокола сервер должен закрыть соединение с кодом [`QWebSocketProtocol::CloseCodeDatatypeNotSupported`](https://doc.qt.io/qt-6/qwebsocketprotocol.html#CloseCode-enum)

## Схема сообщения

### Схема запроса

Каждый запрос содержит идентификатор версии протокола, название метода и уникальный идентификатор запроса. Идентификаторы запросов переиспользуются во время сессии.

```yaml
type: object
required: [id, methodId, params]
additionalProperties: false
properties:
  methodId:
    type: string
    pattern: '^[a-z][.a-zA-Z0-9]*$'
    maxLength: 63
    description: идентификатор метода
  id:
    type: integer
    minimum: 0
    maximum: 255
    description: >-
      идентификатор запроса, могут повторяться в одной сессии;
      не может существовать 2 активных запроса (сервер еще отправил все ответы);
      лимит значений id нужен для ограничения числа одновременных запросов 
  params:
    type: object
    propertyNames:
      pattern: '^[a-z][a-zA-Z0-9]*$'
```

### Схема ответа

На каждый запрос сервер отвечает одним или более сообщением, каждый ответ содержит идентификатор запроса `id`, флаг `next`, отмечающий, следует ли ожидать дополнительных ответов от сервера и *один из параметров* `result` или `error`. Запросы на которые сервер всегда отвечает одним ответом называются *скалярными*, иначе -- *потоковыми*.

Ошибка состоит из целочисленного кода `code` и дополнительной отладочной информации `extra`.

```yaml
type: object
required: [id]
additionalProperties: false
properties:
  id:
    type: integer
    minimum: 0
    maximum: 255
    description: идентификатор запроса
  next:
    default: false
    type: boolean
    description: флаг отмечающий наличие дополнительных ответов
  result:
    type: object
    propertyNames:
      pattern: '^[a-z][a-zA-Z0-9]*$'
    description: тело ответа, исключает поле error
  error:
    type: object
    required: [code]
    additionalProperties: false
    properties:
      code:
        type: integer
        minimum: 0
        maximum: 32767
        description: код ошибки
      extra:
        type: object
        propertyNames:
          pattern: '^[a-z][.a-zA-Z0-9]*$'
        description: дополнительные отладочные аттрибуты
    description: описание ошибки, исключает поле result
```

## Общие ошибки

| Код   | Название                | Отображаемый текст | Примечание                                                         |
| :---- | :---------------------- | :----------------- | :----------------------------------------------------------------- |
| 01000 | `BAD_REQUEST`           | *нет*              | в `extra` можно указать некорректное поле и текст ошибки валидации |
| 01001 | `VERSION_MISMATCH`      | *нет*              |                                                                    |
| 01002 | `INTERNAL_SERVER_ERROR` | *нет*              | необрабатываемая ошибка сервера                                    |

## Методы

### Проверка здоровья сервера

|                |                |
| :------------- | :------------- |
| **Название**   | `ping`         |
| **Тип метода** | скалярный      |
| **Описание**   | пингуем сервер |

**Cхема запроса**:

```yaml
type: object
additionalProperties: false
properties: {}
```

**Cхема ответа**:

```yaml
type: object
additionalProperties: false
properties: {}
```

### Методы работы с АЦП

|                |                                    |
| :------------- | :--------------------------------- |
| **Название**   | `signalRecording.describeChannels` |
| **Тип метода** | скалярный                          |
| **Описание**   | получение информации об АЦП        |

**Cхема запроса**:

```yaml
type: object
additionalProperties: false
properties: {}
```

**Схема ответа**:

```yaml
type: object
required: [deviceType, channelsCount, samplingRate]
additionalProperties: false
properties:
  deviceType:
    type: string
    maxLength: 255
    description: тип устройства
  channelsCount:
    type: integer
    minimum: 0
    maximum: 255      
  samplingRate:
    type: integer
    minimum: 0
    maximum: 2147483647
    description: частота дискретизации АЦП
```

### Старт записи аналогового сигнала

|                |                                             |
| :------------- | :------------------------------------------ |
| **Название**   | `signalRecording.start`                     |
| **Тип метода** | потоковый                                   |
| **Описание**   | начало записи сигнала с аналогового датчика |

Начинает запись сигнала с датчиков по указанным каналам. Для каждого канала указывается коэффициент усиления и, опционально, идентификатор для сохранения записи. Дополнительно сигнал может передаваться в клиент для визуализации.

Сигнал преобразуется в двух местах перед передачей клиенту

1. В `channels.[*].visualTransformType`, `channels.[*].visualTransformParams` задаются несжимающие трансформации, позволяющие вычислять производные характеристки сигнала (например: прямой спектр)
2. В `visual.rollupStrategy`, `visual.rollupFrameSize`, `visual.rollupParams` задаются параметры свертки сигнала (здесь под сверткой подразумевается уменьшение числа точек, по которым строится визуализация сигнала).

**Схема запроса**:

```yaml
type: object
required: [channels, visual]
additionalProperties: false
properties:
  recordingId:
    type: integer
    minimum: 0
    maximum: 255
    description: идентификатор записи сигнала
  visual:
    default: null
    oneOf:
      - type: "null"
      - $ref: "#/$defs/visual"
    description: настройки передачи сигнала для визуализации, если не указан или `null`, то данные для визуализации не передаются
  channels:
    type: array
    item:
      $ref: "#/$defs/channel"
    description: список каналов, с которых ведется запись
$defs:
  visual:
    type: object
    required: [intervalMillis]
    properties:
      intervalMillis:
        type: integer
        minimum: 0
        maximum: 10000
        description: интервал между ответами в миллисекундах
      rollupStrategy:
        default: null
        oneOf:
          - type: "null"
          - type: string
            pattern: '^[a-z][.a-zA-Z0-9]*$'
            maxLength: 63
        description: название алгоритма сжатия графика
      rollupParams:
        default: null
        oneOf:
          - type: "null"
          - type: object
            propertyNames:
              pattern: '^[a-z][.a-zA-Z0-9]*$'
        description: опциональные параметры свертки графика
  channel:
    type: object
    required: [channelId]
    additionalProperties: false
    properties:
      channelId:
        type: integer
        minimum: 0
        maximum: 255
        description: идентификатор канала, не может повторяться в одном запросе, соответствует номеру канала в АЦП
      recordingDataId:
        default: null
        oneOf:
          - type: "null"
          - type: string
            maxLength: 63
            pattern: '^[a-zA-Z][A-Za-z0-9]*$'
        description: глобально уникальный идентификатор записанного сигнала (маппится на файл в папке)
      gainMultiplier:
        default: 1
        type: number
        minimum: 0
        maximum: 1
        description: коэффициент усиления датчика      
      visualTransformType:
        default: null
        oneOf:
          - type: "null"
          - type: string
        description: наименование преобразования сигнала для визуализации         
      visualTransformParams:
        default: null
        oneOf:
          - type: "null"
          - type: object
        description: параметры трансформации
```

* Если visualTransformType не задан, на визуализацию поступает сигнал без преобразований.
* Если recordingPath не задан запись в файл не ведётся.
* Всё, что с приставкой visual, то управляет процессом визуализации.

**Схема ответа**:

```yaml
type: object
required: [frames]
additionalProperties: false
properties:
  frames:
    type: object
    patternProperties:
      '\d+':
        type: array
        item:
          $ref: "#/$defs/recordingFrame"
    description: отображение id канала в набор измерений
defs:
  recordingFrame:
    type: array
    item:
      type: number
    description: точки одного измерения
```

|                |                            |
| :------------- | :------------------------- |
| **Название**   | `signalRecording.stop`     |
| **Тип метода** | скалярный                  |
| **Описание**   | сигнал об окончании записи |

**Схема запроса**:

```yaml
type: object
required: [channelId]
additionalProperties: false
properties:
  recordingId:
    type: integer
    minimum: 0
    maximum: 255
    description: идентификатор записи сигнала
```

**Схема ответа**:

```yaml
type: object
additionalProperties: false
properties:
  recordingSizeBytes:
    default: null
    oneOf:
      - type: "null"
      - type: integer
        minimum: 0
    description: размер файла с записанным сигналом, если файл не сохранялся, то `null`
```

#### Коды ошибок

| Код   | Название                  | Отображаемый текст | Примечание                                                                     |
| :---- | :------------------------ | :----------------- | :----------------------------------------------------------------------------- |
| 02000 | `CHANNEL_BUSY`            | *нет*              | возвращается при попытке начать запись с канала, с которого уже ведется запись |
| 02001 | `CHANNEL_ALREADY_STOPPED` | *нет*              | попытка остановить запись в канале, в котором не велась запись                 |
| 02002 | `RECORD_SAVE_OS_ERROR`    | *нет*              | ошибка записи в файл                                                           |

### Построение графиков записанных сигналов

|                |                                 |
| :------------- | :------------------------------ |
| **Название**   | `plotter.start`                 |
| **Тип метода** | скалярный                       |
| **Описание**   | инициализируем сессию рисования |

**Схема запроса**:

```yaml
type: object
required: [path]
additionalProperties: false
properties:
  dataId:
    type: string
    maxLength: 63
    pattern: '^[a-zA-Z][A-Za-z0-9]*$'
    description: идентификатор записанного сигнала
  rollupStrategy:
    default: null
    oneOf:
      - type: "null"
      - type: string
        pattern: '^[a-z][.a-zA-Z0-9]*$'
        maxLength: 63
    description: название алгоритма сжатия графика
  rollupParams:
    default: null
    oneOf:
      - type: "null"
      - type: object
        propertyNames:
          pattern: '^[a-z][.a-zA-Z0-9]*$'
    description: опциональные параметры свертки графика
```

**Схема ответа**:

```yaml
type: object
required: [plotterId]
additionalProperties: false
properties:
  plotterId:
    type: integer
    minimum: 0
    maximum: 255
    description: идентификатор графика
  range:
    type: array
    minItems: 2
    maxItems: 2
    item:
      type: number
    description: временной интервал, содержащийся в файле
```

|                |                            |
| :------------- | :------------------------- |
| **Название**   | `plotter.stop`             |
| **Тип метода** | скалярный                  |
| **Описание**   | закрываем сессию рисования |

**Схема запроса**:

```yaml
type: object
required: [path]
additionalProperties: false
properties:
  plotterId:
    type: integer
    minimum: 0
    maximum: 255
    description: идентификатор графика
```

**Схема ответа**:

```yaml
type: object
additionalProperties: false
properties: {}
```

|                |                            |
| :------------- | :------------------------- |
| **Название**   | `plotter.plot`             |
| **Тип метода** | скалярный                  |
| **Описание**   | закрываем сессию рисования |

**Схема запроса**:

```yaml
type: object
required: [plotterId, range]
additionalProperties: false
properties:
  plotterId:
    type: integer
    minimum: 0
    maximum: 255
    description: идентификатор графика
  range:
    type: array
    minItems: 2
    maxItems: 2
    item:
      type: number
    description: интервал отрисовки
```

**Схема ответа**:

```yaml
type: object
additionalProperties: false
properties:
  frame:
    type: array
    item:
      $ref: "#/$defs/point"
    description: набор точек для отрисовки
$defs:
  point:
    type: object
    additionalProperties: false
    required: [x, y]
    properties:
      x:
        type: number
      y:
        type: number
```

|                |                                                      |
| :------------- | :--------------------------------------------------- |
| **Название**   | `plotter.selectStationaryIntervals`                  |
| **Тип метода** | скалярный                                            |
| **Описание**   | выбираем стационарные интервалы в записанном сигнале |

**Схема запроса**:

```yaml
type: object
required: [plotterId, range]
additionalProperties: false
properties:
  plotterId:
    type: integer
    minimum: 0
    maximum: 255
    description: идентификатор графика
```

**Схема ответа**:

```yaml
type: object
additionalProperties: false
properties:
  ranges:
    type: array
    item:
      $ref: "#/$defs/range"
    description: последовательность непересекающихся интервалов, содержащих стационарные части сигнала
$defs:
  range:
    type: array
    minItems: 2
    maxItems: 2
    item:
      type: number
```

### Управление записанными сигналами

|                |                                       |
| :------------- | :------------------------------------ |
| **Название**   | `signalData.list`                     |
| **Тип метода** | потоковый                             |
| **Описание**   | перечисление всех записанных сигналов |

**Схема запроса**:

```yaml
type: object
additionalProperties: false
properties:
  dataIdPattern:
    default: null
    oneOf:
      - type: "null"
      - type: string
    description: регулярное выражение для фильтра записей
```

**Схема ответа**:

```yaml
type: object
additionalProperties: false
required: [dataId, sizeBytes, creationDateTime, modificationDateTime]
properties:
  dataId:
    type: string
    maxLength: 63
    pattern: '^[a-zA-Z][A-Za-z0-9]*$'
    description: идентификатор записанного сигнала
  sizeBytes:
    type: integer
    minimum: 0
    description: размер файла
  creationDateTime:
    type: string
    format: date-time
    description: время создания
  modificationDateTime:
    type: string
    format: date-time
    description: время модификации
```

|                |                           |
| :------------- | :------------------------ |
| **Название**   | `signalData.read`         |
| **Тип метода** | потоковый                 |
| **Описание**   | отправка файла с сигналом |

**Схема запроса**:

```yaml
type: object
additionalProperties: false
required: [dataId]
properties:
  dataId:
    type: string
    maxLength: 63
    pattern: '^[a-zA-Z][A-Za-z0-9]*$'
    description: идентификатор записанного сигнала
```

**Схема ответа**:

```yaml
type: object
additionalProperties: false
required: [chunk]
properties:
  chunk:
    type: string
    pattern: '[^-A-Za-z0-9+/=]|=[^=]|={3,}$'
    description: порция файла, закодированная в Base64
```

|                |                           |
| :------------- | :------------------------ |
| **Название**   | `signalData.remove`       |
| **Тип метода** | скалярный                 |
| **Описание**   | удаление файла с сигналом |

**Схема запроса**:

```yaml
type: object
additionalProperties: false
required: [dataId]
properties:
  dataId:
    type: string
    maxLength: 63
    pattern: '^[a-zA-Z][A-Za-z0-9]*$'
    description: идентификатор записанного сигнала
```

**Схема ответа**:

```yaml
type: object
additionalProperties: false
properties: {}
```

|                |                                           |
| :------------- | :---------------------------------------- |
| **Название**   | `signalData.transform`                    |
| **Тип метода** | скалярный                                 |
| **Описание**   | выполняем преобразование файла с сигналом |

**Схема запроса**:

```yaml
type: object
required: [sourceDataId, destinationDataId, transformId]
additionalProperties: false
properties:
  sourceDataId:
    type: string
    maxLength: 63
    pattern: '^[a-zA-Z][A-Za-z0-9]*$'
    description: идентификатор записанного сигнала
  destinationDataId:
    type: string
    maxLength: 63
    pattern: '^[a-zA-Z][A-Za-z0-9]*$'
    description: идентификатор сохранения преобразованного сигнала
  transformId:
    type: string
    maxLength: 63
    pattern: '^[a-zA-Z][A-Za-z0-9]*$'
    description: название трансформации
  transformParams:
    default: null
    oneOf:
      - type: "null"
      - type: object
        propertyNames:
          pattern: '^[a-z][.a-zA-Z0-9]*$'
    description: параметры трансформации
```

**Схема ответа**:

```yaml
type: object
additionalProperties: false
properties:
  destinationSizeBytes:
    type: integer
    minimum: 0
    description: размер файла с записанным сигналом
```

## Алгоритмы свертки

TODO

## Алгоритмы преобразования сигнала

Здесь будет список возможных преобразований с параметрами.
