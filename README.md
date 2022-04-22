# gtlab-mts-websocket-api
Реализация WEBSOCKET-API между UI (by МТС) и BACK-END (by GTLAB.Диагностика)

# Докоментация по API

## Схема сообщения

Обмен сообщениями происходит через WebSocket, сообщения кодируются в JSON и должны соответствовать следующим схемам. Схемы представлены в формате [JSONSchema](https://json-schema.org/understanding-json-schema/index.html).

### Схема запроса

Каждый запрос содержит идентификатор версии протокола, название метода и уникальный идентификатор запроса. Идентификаторы запросов переиспользуются во время сессии.

```yaml
type: object
required: [version, id, methodId, params]
additionalProperties: false
properties:
  version:
    type: string
    maxLength: 63
    description: строка с версией протокола, нужна чтобы сервер мог проверить совместимость клиента с собой
  methodId:
    type: string
    pattern: '^[a-z][.a-zA-Z0-9]*$'
    maxLength: 63
    description: идентификатор метода
  id:
    type: integer
    minimum: 0
    maximum: 255
    description: идентификатор запроса, могут повторяться в одной сессии; не может существовать 2 активных запроса (сервер еще отправил все ответы); лимит значений id нужен для ограничения числа одновременных запросов 
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

|                |                                                |
| :------------- | :--------------------------------------------- |
| **Название**   | `signalRecording.describeChannels`             |
| **Тип метода** | скалярный                                      |
| **Описание**   | получение информации об АЦП                    |

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

**Схема запроса**:

```yaml
type: object
required: [channels, frameIntervalMillis]
additionalProperties: false
properties:
  recordingId:
    type: integer
    minimum: 0
    maximum: 255
    description: идентификатор записи сигнала
  measurementsFrame:
    type: integer
    minimum: 0
    maximum: 32767
    description: количество точек возвращаемых в одном ответе
  frameIntervalMillis:
    type: integer
    minimum: 0
    maximum: 32767
    description: интервал между ответами в миллисекундах
  channels:
    type: array
    item:
      $ref: "#/$defs/channel"
    description: список каналов, с которых ведется запись
$defs:
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
      recordingPath:
        default: null
        oneOf:
          - type: "null"
          - type: string
        description: путь для сохранения сигнала       
```
* Если visualTransformType не задан, на визуализацию поступает сигнал без преобразований.
* Если recordingPath не задан запись в файл не ведётся.

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
properties: {}
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
  path:
    type: string
    description: путь к файлу с записанным сигналом
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

### Преобразование сигнала

|                |                                           |
| :------------- | :---------------------------------------- |
| **Название**   | `signalTransform`                         |
| **Тип метода** | скалярный                                 |
| **Описание**   | выполняем преобразование файла с сигналом |

**Схема запроса**:

```yaml
type: object
required: [sourcePath, destinationPath, kind]
additionalProperties: false
properties:
  sourcePath:
    type: string
    description: путь к файлу с записанным сигналом
  destinationPath:
    type: string
    description: путь сохранения преобразованного сигнала
  transformId:
    type: string
    maxLength: 63
    pattern: '^[a-z][A-Za-z]*$'
    description: название трансформации
  transformParams:
    default: null
    oneOf:
      - type: "null"
      - type: object
    description: параметры трансформации
```

**Схема ответа**:

```yaml
type: object
additionalProperties: false
properties: {}
```

#### Конкретные преобразования

Здесь будет список возможных преобразований с параметрами.
